#include "TEECO_System.h"
#include "DataLog.h"

#define QUESCAN_GPS_FIELD_STATUS       2
#define QUESCAN_GPS_FIELD_LATITUDE     3
#define QUESCAN_GPS_FIELD_NS           4
#define QUESCAN_GPS_FIELD_LONGITUDE    5
#define QUESCAN_GPS_FIELD_EW           6
#define QUESCAN_GPS_BUFFER_LIMIT       256

static void QuescanGPS_ResetReceive(int comPort)
{
    gv.nFlag_ReceiveStart[comPort] = 0;
    gv.nReceiveBufferCount[comPort] = 0;
}

static int QuescanGPS_CopyField(const unsigned char *sentence, int fieldNo, char *out, int outSize)
{
    int field = 0;
    int i = 0;
    int outIndex = 0;

    if(outSize <= 0) {
        return 0;
    }

    out[0] = 0;

    while(sentence[i] != 0 && sentence[i] != '\r' && sentence[i] != '\n') {
        if(sentence[i] == ',') {
            field++;
            i++;
            continue;
        }

        if(sentence[i] == '*') {
            break;
        }

        if(field == fieldNo) {
            if(outIndex >= (outSize - 1)) {
                break;
            }
            out[outIndex++] = (char)sentence[i];
        }

        i++;
    }

    out[outIndex] = 0;
    return (outIndex > 0);
}

static float QuescanGPS_NmeaToDegree(const char *value)
{
    float nmea = (float)atof(value);
    int degree = (int)(nmea / 100.0f);
    float minute = nmea - ((float)degree * 100.0f);
    float gps = (float)degree + (minute / 60.0f);

    return gps;
}

static void QuescanGPS_ProcessRmc(int comPort)
{
    unsigned char *sentence = gv.aItemReceiveBuffer[comPort];
    char status[2];
    char latitude[16];
    char ns[2];
    char longitude[16];
    char ew[2];

    if(sentence[0] != '$' ||
       sentence[1] != 'G' ||
       sentence[2] != 'P' ||
       sentence[3] != 'R' ||
       sentence[4] != 'M' ||
       sentence[5] != 'C') {
        return;
    }

    if(QuescanGPS_CopyField(sentence, QUESCAN_GPS_FIELD_STATUS, status, sizeof(status)) == 0) {
        return;
    }

    if(status[0] != 'A') {
        return;
    }

    if(QuescanGPS_CopyField(sentence, QUESCAN_GPS_FIELD_LATITUDE, latitude, sizeof(latitude)) == 0 ||
       QuescanGPS_CopyField(sentence, QUESCAN_GPS_FIELD_NS, ns, sizeof(ns)) == 0 ||
       QuescanGPS_CopyField(sentence, QUESCAN_GPS_FIELD_LONGITUDE, longitude, sizeof(longitude)) == 0 ||
       QuescanGPS_CopyField(sentence, QUESCAN_GPS_FIELD_EW, ew, sizeof(ew)) == 0) {
        return;
    }

    if(ns[0] != 'N' || ew[0] != 'E') {
        return;
    }

    flash.Uart[comPort]->SensorPV[0] = QuescanGPS_NmeaToDegree(latitude);
    flash.Uart[comPort]->SensorPV[1] = QuescanGPS_NmeaToDegree(longitude);
}

int ReceiveQuescanGPS(int ascii, int comPort)
{
    if(comPort < 0 || comPort >= USART_MAX) {
        return 0;
    }

    if(ascii == '$') {
        gv.nFlag_ReceiveStart[comPort] = 1;
        gv.nReceiveBufferCount[comPort] = 0;
        gv.aItemReceiveBuffer[comPort][gv.nReceiveBufferCount[comPort]++] = (unsigned char)ascii;
    }
    else if(gv.nFlag_ReceiveStart[comPort] == 1) {
        if(gv.nReceiveBufferCount[comPort] >= (QUESCAN_GPS_BUFFER_LIMIT - 1)) {
            QuescanGPS_ResetReceive(comPort);
            gv.nFlag_ReceiveDataProcess[comPort] = DISABLE;
            return 1;
        }

        gv.aItemReceiveBuffer[comPort][gv.nReceiveBufferCount[comPort]++] = (unsigned char)ascii;
        gv.aItemReceiveBuffer[comPort][gv.nReceiveBufferCount[comPort]] = 0;

        if(ascii == 0x0A) {
            QuescanGPS_ProcessRmc(comPort);
            QuescanGPS_ResetReceive(comPort);
        }
    }

    return 1;
}
