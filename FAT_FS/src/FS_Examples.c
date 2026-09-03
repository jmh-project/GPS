#ifdef __cplusplus
 extern "C" {
#endif

#include "TEECO_System.h"
#include "ff.h"
#include "BSP_DS1302.h"

//참조용 파일이다. 첸의 응용 프로그램
#if 0
/*------------------------------------------------------------/
/ Open or create a file in append mode
/------------------------------------------------------------*/

FRESULT open_append (
    FIL* fp,            /* [OUT] File object to create */
    const char* path    /* [IN]  File name to be opened */
)
{
    FRESULT fr;

    /* Opens an existing file. If not exist, creates a new file. */
    fr = f_open(fp, path, FA_WRITE | FA_OPEN_ALWAYS);
    if (fr == FR_OK) {
        /* Seek to end of the file to append data */
        fr = f_lseek(fp, f_size(fp));
        if (fr != FR_OK)
            f_close(fp);
    }
    return fr;
}

int open_append_Test (void)
{
    FRESULT fr;
    FATFS fs;
    FIL fil;

    /* Open or create a log file and ready to append */
    f_mount(&fs, "", 0);
    fr = open_append(&fil, "logfile.txt");
    if (fr != FR_OK) return 1;

    /* Append a line */
    f_printf(&fil, "%02u/%02u/%u, %2u:%02u\n", Mday, Mon, Year, Hour, Min);

    /* Close the file */
    f_close(&fil);

    return 0;
}

/*------------------------------------------------------------/
/ Remove all contents of a directory
/ This function works regardless of _FS_RPATH.
/------------------------------------------------------------*/
FRESULT empty_directory (
    char* path      /* Working buffer filled with start directory */
)
{
    UINT i, j;
    FRESULT fr;
    DIR dir;
    FILINFO fno;

#if _USE_LFN
    fno.lfname = 0; /* Disable LFN output */
#endif
    fr = f_opendir(&dir, path);
    if (fr == FR_OK) 
	{
        for (i = 0; path[i]; i++) ;	//포인터이동
			path[i++] = '/';
        for (;;) 
		{
            fr = f_readdir(&dir, &fno);
            if (fr != FR_OK || !fno.fname[0]) break;
            if (_FS_RPATH && fno.fname[0] == '.') continue;
            j = 0;
            do
                path[i+j] = fno.fname[j];
            while (fno.fname[j++]);
            if (fno.fattrib & AM_DIR) 
			{
                fr = empty_directory(path);
                if (fr != FR_OK) break;
            }
            fr = f_unlink(path);
            if (fr != FR_OK) break;
        }
        path[--i] = '\0';
        closedir(&dir);
    }
    return fr;
}

int empty_directory_Test (void)
{
    FRESULT fr;
    FATFS fs;
    char buff[64];    /* Working buffer */

    f_mount(&fs, "", 0);

    strcpy(buff, "/");  /* Directory to be emptied */
    fr = empty_directory(buff);

    if (fr) 
	{
        Dprintf("Function failed. (%u)\n", fr);
        return fr;
    } 
	else 
	{
        Dprintf("All contents in the %s are successfully removed.\n", buff);
        return 0;
    }
}

/*----------------------------------------------------------------------/
/ Allocate a contiguous area to the file
/-----------------------------------------------------------------------/
/ This function checks if the file is contiguous with desired size.
/ If not, a block of contiguous sectors is allocated to the file.
/ If the file has been opened without FA_WRITE flag, it only checks if
/ the file is contiguous and returns the resulut. */

#if _FATFS != 80367 /* Check if R0.10c */
#error This function may not be compatible with this revision of FatFs module.
#endif

/* Declarations of FatFs internal functions accessible from applications.
/  This is intended to be used for disk checking/fixing or dirty hacks :-) */
DWORD clust2sect (FATFS* fs, DWORD clst);
DWORD get_fat (FATFS* fs, DWORD clst);
FRESULT put_fat (FATFS* fs, DWORD clst, DWORD val);


DWORD allocate_contiguous_clusters (    /* Returns the first sector in LBA (0:error or not contiguous) */
    FIL* fp,    /* Pointer to the open file object */
    DWORD len   /* Number of bytes to allocate */
)
{
    DWORD csz, tcl, ncl, ccl, cl;


    if (f_lseek(fp, 0) || !len)     /* Check if the given parameters are valid */
        return 0;
    csz = 512UL * fp->fs->csize;    /* Cluster size in unit of byte (assuming 512 bytes/sector) */
    tcl = (len + csz - 1) / csz;    /* Total number of clusters required */
    len = tcl * csz;                /* Round-up file size to the cluster boundary */

    /* Check if the existing cluster chain is contiguous */
    if (len == fp->fsize) {
        ncl = 0; ccl = fp->sclust;
        do {
            cl = get_fat(fp->fs, ccl);  /* Get the cluster status */
            if (cl + 1 < 3) return 0;   /* Hard error? */
            if (cl != ccl + 1 &&; cl < fp->fs->n_fatent) break;  /* Not contiguous? */
            ccl = cl;
        } while (++ncl < tcl);
        if (ncl == tcl)             /* Is the file contiguous? */
            return clust2sect(fp->fs, fp->sclust);  /* Return file start sector */
    }
#if _FS_READONLY
    return 0;
#else
    if (f_truncate(fp)) return 0;   /* Remove the existing chain */

    /* Find a free contiguous area */
    ccl = cl = 2; ncl = 0;
    do {
        if (cl >= fp->fs->n_fatent) return 0;   /* No contiguous area is found. */
        if (get_fat(fp->fs, cl)) {  /* Encounterd a cluster in use */
            do {    /* Skip the block of used clusters */
                cl++;
                if (cl >= fp->fs->n_fatent) return 0;   /* No contiguous area is found. */
            } while (get_fat(fp->fs, cl));
            ccl = cl; ncl = 0;
        }
        cl++; ncl++;
    } while (ncl < tcl);

    /* Create a contiguous cluster chain */
    fp->fs->last_clust = ccl - 1;
    if (f_lseek(fp, len)) return 0;

    return clust2sect(fp->fs, fp->sclust);  /* Return file start sector */
#endif
}


int allocate_contiguous_clusters_Test (void)
{
    FRESULT fr;
    DRESULT dr;
    FATFS fs;
    FIL fil;
    DWORD org;


    /* Open or create a file */
    f_mount(&fs, "", 0);
    fr = f_open(&fil, "swapfile.sys", FA_READ | FA_WRITE | FA_OPEN_ALWAYS);
    if (fr) return 1;

    /* Check if the file is 64MB in size and occupies a contiguous area.
    /  If not, a contiguous area will be re-allocated to the file. */
    org = allocate_contiguous_clusters(&fil, 0x4000000);
    if (!org) {
        printf("Function failed due to any error or insufficient contiguous area.\n");
        f_close(&fil);
        return 1;
    }

    /* Now you can read/write the file with disk functions bypassing the file system layer. */

    dr = disk_write(fil.fs->drv, Buff, org, 1024);   /* Write 512KiB from top of the file */

    ...

    f_close(&fil);
    return 0;
}

/*----------------------------------------------------------------------/
/ Low level disk I/O module function checker
/-----------------------------------------------------------------------/
/ WARNING: The data on the target drive will be lost!
*/

#include <stdio.h>
#include <string.h>
#include "ff.h"
#include "diskio.h"


static
DWORD pn (
    DWORD pns
)
{
    static DWORD lfsr;
    UINT n;


    if (pns) {
        lfsr = pns;
        for (n = 0; n < 32; n++) pn(0);
    }
    if (lfsr & 1) {
        lfsr >>= 1;
        lfsr ^= 0x80200003;
    } else {
        lfsr >>= 1;
    }
    return lfsr;
}


int test_diskio (
    BYTE pdrv,      /* Physical drive number to be checked (all data on the drive will be lost) */
    UINT ncyc,      /* Number of test cycles */
    DWORD* buff,    /* Pointer to the working buffer */
    UINT sz_buff    /* Size of the working buffer in unit of byte */
)
{
    UINT n, cc, ns;
    DWORD sz_drv, lba, lba2, pns = 1;
    WORD sz_sect, sz_eblk;
    BYTE *pbuff = (BYTE*)buff;
    DSTATUS ds;
    DRESULT dr;

    printf("test_diskio(%u, %u, 0x%08X, 0x%08X)\n", pdrv, ncyc, (UINT)buff, sz_buff);

    if (sz_buff < _MAX_SS + 4) {
        printf("Insufficient work area to test.\n");
        return 1;
    }

    for (cc = 1; cc <= ncyc; cc++) {
        printf("**** Test cycle %u of %u start ****\n", cc, ncyc);

        /* Initialization */
        printf(" disk_initalize(%u)", pdrv);
        ds = disk_initialize(pdrv);
        if (ds & STA_NOINIT) {
            printf(" - failed.\n");
            return 2;
        } else {
            printf(" - ok.\n");
        }

        /* Get drive size */
        printf("**** Get drive size ****\n");
        printf(" disk_ioctl(%u, GET_SECTOR_COUNT, 0x%08X)", pdrv, (UINT)&sz_drv);
        sz_drv = 0;
        dr = disk_ioctl(pdrv, GET_SECTOR_COUNT, &sz_drv);
        if (dr == RES_OK) {
            printf(" - ok.\n");
        } else {
            printf(" - failed.\n");
            return 3;
        }
        if (sz_drv < 128) {
            printf("Failed: Insufficient drive size to test.\n");
            return 4;
        }
        printf(" Number of sectors on the drive %u is %lu.\n", pdrv, sz_drv);

#if _MAX_SS != _MIN_SS
        /* Get sector size */
        printf("**** Get sector size ****\n");
        printf(" disk_ioctl(%u, GET_SECTOR_SIZE, 0x%X)", pdrv, (UINT)&sz_sect);
        sz_sect = 0;
        dr = disk_ioctl(pdrv, GET_SECTOR_SIZE, &sz_sect);
        if (dr == RES_OK) {
            printf(" - ok.\n");
        } else {
            printf(" - failed.\n");
            return 5;
        }
        printf(" Size of sector is %u bytes.\n", sz_sect);
#else
        sz_sect = _MAX_SS;
#endif

        /* Get erase block size */
        printf("**** Get block size ****\n");
        printf(" disk_ioctl(%u, GET_BLOCK_SIZE, 0x%X)", pdrv, (UINT)&sz_eblk);
        sz_eblk = 0;
        dr = disk_ioctl(pdrv, GET_BLOCK_SIZE, &sz_eblk);
        if (dr == RES_OK) {
            printf(" - ok.\n");
        } else {
            printf(" - failed.\n");
        }
        if (dr == RES_OK || sz_eblk >= 2) {
            printf(" Size of the erase block is %u sectors.\n", sz_eblk);
        } else {
            printf(" Size of the erase block is unknown.\n");
        }

        /* Single sector write test */
        printf("**** Single sector write test 1 ****\n");
        lba = 0;
        for (n = 0, pn(pns); n < sz_sect; n++) pbuff[n] = (BYTE)pn(0);
        printf(" disk_write(%u, 0x%X, %lu, 1)", pdrv, (UINT)pbuff, lba);
        dr = disk_write(pdrv, pbuff, lba, 1);
        if (dr == RES_OK) {
            printf(" - ok.\n");
        } else {
            printf(" - failed.\n");
            return 6;
        }
        printf(" disk_ioctl(%u, CTRL_SYNC, NULL)", pdrv);
        dr = disk_ioctl(pdrv, CTRL_SYNC, 0);
        if (dr == RES_OK) {
            printf(" - ok.\n");
        } else {
            printf(" - failed.\n");
            return 7;
        }
        memset(pbuff, 0, sz_sect);
        printf(" disk_read(%u, 0x%X, %lu, 1)", pdrv, (UINT)pbuff, lba);
        dr = disk_read(pdrv, pbuff, lba, 1);
        if (dr == RES_OK) {
            printf(" - ok.\n");
        } else {
            printf(" - failed.\n");
            return 8;
        }
        for (n = 0, pn(pns); n < sz_sect && pbuff[n] == (BYTE)pn(0); n++) ;
        if (n == sz_sect) {
            printf(" Data matched.\n");
        } else {
            printf("Failed: Read data differs from the data written.\n");
            return 10;
        }
        pns++;

        /* Multiple sector write test */
        printf("**** Multiple sector write test ****\n");
        lba = 1; ns = sz_buff / sz_sect;
        if (ns > 4) ns = 4;
        for (n = 0, pn(pns); n < (UINT)(sz_sect * ns); n++) pbuff[n] = (BYTE)pn(0);
        printf(" disk_write(%u, 0x%X, %lu, %u)", pdrv, (UINT)pbuff, lba, ns);
        dr = disk_write(pdrv, pbuff, lba, ns);
        if (dr == RES_OK) {
            printf(" - ok.\n");
        } else {
            printf(" - failed.\n");
            return 11;
        }
        printf(" disk_ioctl(%u, CTRL_SYNC, NULL)", pdrv);
        dr = disk_ioctl(pdrv, CTRL_SYNC, 0);
        if (dr == RES_OK) {
            printf(" - ok.\n");
        } else {
            printf(" - failed.\n");
            return 12;
        }
        memset(pbuff, 0, sz_sect * ns);
        printf(" disk_read(%u, 0x%X, %lu, %u)", pdrv, (UINT)pbuff, lba, ns);
        dr = disk_read(pdrv, pbuff, lba, ns);
        if (dr == RES_OK) {
            printf(" - ok.\n");
        } else {
            printf(" - failed.\n");
            return 13;
        }
        for (n = 0, pn(pns); n < (UINT)(sz_sect * ns) && pbuff[n] == (BYTE)pn(0); n++) ;
        if (n == (UINT)(sz_sect * ns)) {
            printf(" Data matched.\n");
        } else {
            printf("Failed: Read data differs from the data written.\n");
            return 14;
        }
        pns++;

        /* Single sector write test (misaligned memory address) */
        printf("**** Single sector write test 2 ****\n");
        lba = 5;
        for (n = 0, pn(pns); n < sz_sect; n++) pbuff[n+3] = (BYTE)pn(0);
        printf(" disk_write(%u, 0x%X, %lu, 1)", pdrv, (UINT)(pbuff+3), lba);
        dr = disk_write(pdrv, pbuff+3, lba, 1);
        if (dr == RES_OK) {
            printf(" - ok.\n");
        } else {
            printf(" - failed.\n");
            return 15;
        }
        printf(" disk_ioctl(%u, CTRL_SYNC, NULL)", pdrv);
        dr = disk_ioctl(pdrv, CTRL_SYNC, 0);
        if (dr == RES_OK) {
            printf(" - ok.\n");
        } else {
            printf(" - failed.\n");
            return 16;
        }
        memset(pbuff+5, 0, sz_sect);
        printf(" disk_read(%u, 0x%X, %lu, 1)", pdrv, (UINT)(pbuff+5), lba);
        dr = disk_read(pdrv, pbuff+5, lba, 1);
        if (dr == RES_OK) {
            printf(" - ok.\n");
        } else {
            printf(" - failed.\n");
            return 17;
        }
        for (n = 0, pn(pns); n < sz_sect && pbuff[n+5] == (BYTE)pn(0); n++) ;
        if (n == sz_sect) {
            printf(" Data matched.\n");
        } else {
            printf("Failed: Read data differs from the data written.\n");
            return 18;
        }
        pns++;

        /* 4GB barrier test */
        printf("**** 4GB barrier test ****\n");
        if (sz_drv >= 128 + 0x80000000 / (sz_sect / 2)) {
            lba = 6; lba2 = lba + 0x80000000 / (sz_sect / 2);
            for (n = 0, pn(pns); n < (UINT)(sz_sect * 2); n++) pbuff[n] = (BYTE)pn(0);
            printf(" disk_write(%u, 0x%X, %lu, 1)", pdrv, (UINT)pbuff, lba);
            dr = disk_write(pdrv, pbuff, lba, 1);
            if (dr == RES_OK) {
                printf(" - ok.\n");
            } else {
                printf(" - failed.\n");
                return 19;
            }
            printf(" disk_write(%u, 0x%X, %lu, 1)", pdrv, (UINT)(pbuff+sz_sect), lba2);
            dr = disk_write(pdrv, pbuff+sz_sect, lba2, 1);
            if (dr == RES_OK) {
                printf(" - ok.\n");
            } else {
                printf(" - failed.\n");
                return 20;
            }
            printf(" disk_ioctl(%u, CTRL_SYNC, NULL)", pdrv);
            dr = disk_ioctl(pdrv, CTRL_SYNC, 0);
            if (dr == RES_OK) {
            printf(" - ok.\n");
            } else {
                printf(" - failed.\n");
                return 21;
            }
            memset(pbuff, 0, sz_sect * 2);
            printf(" disk_read(%u, 0x%X, %lu, 1)", pdrv, (UINT)pbuff, lba);
            dr = disk_read(pdrv, pbuff, lba, 1);
            if (dr == RES_OK) {
                printf(" - ok.\n");
            } else {
                printf(" - failed.\n");
                return 22;
            }
            printf(" disk_read(%u, 0x%X, %lu, 1)", pdrv, (UINT)(pbuff+sz_sect), lba2);
            dr = disk_read(pdrv, pbuff+sz_sect, lba2, 1);
            if (dr == RES_OK) {
                printf(" - ok.\n");
            } else {
                printf(" - failed.\n");
                return 23;
            }
            for (n = 0, pn(pns); pbuff[n] == (BYTE)pn(0) && n < (UINT)(sz_sect * 2); n++) ;
            if (n == (UINT)(sz_sect * 2)) {
                printf(" Data matched.\n");
            } else {
                printf("Failed: Read data differs from the data written.\n");
                return 24;
            }
        } else {
            printf(" Test skipped.\n");
        }
        pns++;

        printf("**** Test cycle %u of %u completed ****\n\n", cc, ncyc);
    }

    return 0;
}



int main (int argc, char* argv[])
{
    int rc;
    DWORD buff[512];  /* 2048 byte working buffer */

    /* Check function/compatibility of the physical drive #0 */
    rc = test_diskio(0, 1, buff, sizeof buff);
    if (res) {
        printf("Sorry the function/compatibility test failed.\nFatFs will not work on this disk driver.\n");
    } else {
        printf("Congratulations! The disk I/O layer works well.\n");
    }

    return rc;
}

#endif
#ifdef __cplusplus
}
#endif
