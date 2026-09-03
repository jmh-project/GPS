#include "TEECO_System.h"
#if 0
#define VIDEO_CAMERA_VIEW_X     800
#define VIDEO_CAMERA_VIEW_Y     600

extern volatile int	gStartXH, gStartXL, gStartYH,gStartYL, gAddPoint,gxSize,gySize;

extern uint32_t Camera_ReadRegister(uint16_t RegisterAddr);
extern uint32_t Camera_WriteRegister(uint16_t RegisterAddr, uint8_t RegisterValue);
void Camera_MirrorFlip_Setting( uint16_t mirrorFlipValue )
{
	uint8_t	reg3818=0, reg3621 = 0;
	switch( mirrorFlipValue )
	{
		case 0://Mirror
		{
			reg3818 = Camera_ReadRegister(0x3818);
			reg3818 = reg3818 | 0x00;
			reg3818 = reg3818 & 0x9F;
			Camera_WriteRegister( 0x3818, reg3818 );
			reg3621 = Camera_ReadRegister(0x3621);
			reg3621 = reg3621 | 0x20;
			Camera_WriteRegister( 0x3621, reg3621 );
		}
		break;
		case 1://FLIP
		{
			reg3818 = Camera_ReadRegister(0x3818);
			reg3818 = reg3818 | 0x20;
			reg3818 = reg3818 & 0xBF;
			Camera_WriteRegister( 0x3818, reg3818 );
			reg3621 = Camera_ReadRegister(0x3621);
			reg3621 = reg3621 | 0x20;
			Camera_WriteRegister( 0x3621, reg3621 );
		}
		break;
		case 2://MIRROR/FLIP
		{
			reg3818 = Camera_ReadRegister(0x3818);
			reg3818 = reg3818 | 0x60;
			reg3818 = reg3818 & 0xFF;
			Camera_WriteRegister( 0x3818, reg3818 );
			reg3621 = Camera_ReadRegister(0x3621);
			reg3621 = reg3621 | 0xDF;
			Camera_WriteRegister( 0x3621, reg3621 );
		}
		break;
		case 3://NORMAL
		{
			reg3818 = Camera_ReadRegister(0x3818);
			reg3818 = reg3818 | 0x40;
			reg3818 = reg3818 & 0xDF;
			Camera_WriteRegister( 0x3818, reg3818 );
			reg3621 = Camera_ReadRegister(0x3621);
			reg3621 = reg3621 | 0xDF;
			Camera_WriteRegister( 0x3621, reg3621 );
		}
		break;
		
	}
}
void Camera_Brightness_Setting( uint16_t brightnessValue )
{
	switch( brightnessValue )
	{
		case 0://brightnessValue 0
		{
			Camera_WriteRegister(0x5001 ,0xff);
			Camera_WriteRegister(0x5589 ,0x00);
			Camera_WriteRegister(0x5580 ,0x04);
			Camera_WriteRegister(0x558a ,0x00);
		}
		break;
		case 1://brightnessValue 1
		{
			Camera_WriteRegister(0x5001 ,0xff);
			Camera_WriteRegister(0x5589 ,0x10);
			Camera_WriteRegister(0x5580 ,0x04);
			Camera_WriteRegister(0x558a ,0x00);
		}
		break;
		case 2://brightnessValue 2
		{
			Camera_WriteRegister(0x5001 ,0xff);
			Camera_WriteRegister(0x5589 ,0x20);
			Camera_WriteRegister(0x5580 ,0x04);
			Camera_WriteRegister(0x558a ,0x00);
		}
		break;
		case 3://brightnessValue 3
		{
			Camera_WriteRegister(0x5001 ,0xff);
			Camera_WriteRegister(0x5589 ,0x30);
			Camera_WriteRegister(0x5580 ,0x04);
			Camera_WriteRegister(0x558a ,0x00);
		}
		break;
		case 4://brightnessValue 4
		{
			Camera_WriteRegister(0x5001 ,0xff);
			Camera_WriteRegister(0x5589 ,0x40);
			Camera_WriteRegister(0x5580 ,0x04);
			Camera_WriteRegister(0x558a ,0x00);
		}
		break;
		case 5://brightnessValue -1
		{
			Camera_WriteRegister(0x5001 ,0xff);
			Camera_WriteRegister(0x5589 ,0x10);
			Camera_WriteRegister(0x5580 ,0x04);
			Camera_WriteRegister(0x558a ,0x08);
		}
		break;
		case 6://brightnessValue -2
		{
			Camera_WriteRegister(0x5001 ,0xff);
			Camera_WriteRegister(0x5589 ,0x20);
			Camera_WriteRegister(0x5580 ,0x04);
			Camera_WriteRegister(0x558a ,0x08);
		}
		break;
		case 7://brightnessValue -3
		{
			Camera_WriteRegister(0x5001 ,0xff);
			Camera_WriteRegister(0x5589 ,0x30);
			Camera_WriteRegister(0x5580 ,0x04);
			Camera_WriteRegister(0x558a ,0x08);
		}
		break;
		case 8://brightnessValue -4
		{
			Camera_WriteRegister(0x5001 ,0xff);
			Camera_WriteRegister(0x5589 ,0x40);
			Camera_WriteRegister(0x5580 ,0x04);
			Camera_WriteRegister(0x558a ,0x08);
		}
		break;
	}	
}
void Camera_Saturation_Setting( uint16_t saturationValue )
{
	switch( saturationValue )
	{
		case 0://SaturationValue 0
		{
			Camera_WriteRegister(0x5001 ,0xff);
			Camera_WriteRegister(0x5583 ,0x40);
			Camera_WriteRegister(0x5584 ,0x40);
			Camera_WriteRegister(0x5580 ,0x02);		
		}
		break;
		case 1://SaturationValue 1
		{
			Camera_WriteRegister(0x5001 ,0xff);
			Camera_WriteRegister(0x5583 ,0x50);
			Camera_WriteRegister(0x5584 ,0x50);
			Camera_WriteRegister(0x5580 ,0x02);
		}
		break;
		case 2://SaturationValue 2
		{
			Camera_WriteRegister(0x5001 ,0xff);
			Camera_WriteRegister(0x5583 ,0x60);
			Camera_WriteRegister(0x5584 ,0x60);
			Camera_WriteRegister(0x5580 ,0x02);
		}
		break;
		case 3://SaturationValue 3
		{
			Camera_WriteRegister(0x5001 ,0xff);
			Camera_WriteRegister(0x5583 ,0x70);
			Camera_WriteRegister(0x5584 ,0x70);
			Camera_WriteRegister(0x5580 ,0x02);
		}
		break;
		case 4://SaturationValue 4
		{
			Camera_WriteRegister(0x5001 ,0xff);
			Camera_WriteRegister(0x5583 ,0x80);
			Camera_WriteRegister(0x5584 ,0x80);
			Camera_WriteRegister(0x5580 ,0x02);
		}
		break;
		case 5://SaturationValue -1
		{
			Camera_WriteRegister(0x5001 ,0xff);
			Camera_WriteRegister(0x5583 ,0x30);
			Camera_WriteRegister(0x5584 ,0x30);
			Camera_WriteRegister(0x5580 ,0x02);
		}
		break;
		case 6://SaturationValue -2
		{
			Camera_WriteRegister(0x5001 ,0xff);
			Camera_WriteRegister(0x5583 ,0x20);
			Camera_WriteRegister(0x5584 ,0x20);
			Camera_WriteRegister(0x5580 ,0x02);
		}
		break;
		case 7://SaturationValue -3
		{
			Camera_WriteRegister(0x5001 ,0xff);
			Camera_WriteRegister(0x5583 ,0x10);
			Camera_WriteRegister(0x5584 ,0x10);
			Camera_WriteRegister(0x5580 ,0x02);
		}
		break;
		case 8://SaturationValue -4
		{
			Camera_WriteRegister(0x5001 ,0xff);
			Camera_WriteRegister(0x5583 ,0x00);
			Camera_WriteRegister(0x5584 ,0x00);
			Camera_WriteRegister(0x5580 ,0x02);
		}
		break;
	}
}

void VGA_640_480_PreView( void )
{
	Camera_WriteRegister(0x3103 ,0x93);
	Camera_WriteRegister(0x3008 ,0x82);
	Camera_WriteRegister(0x3017 ,0x7f);
	Camera_WriteRegister(0x3018 ,0xfc);
	Camera_WriteRegister(0x3810 ,0xc2);
	Camera_WriteRegister(0x3615 ,0xf0);
	Camera_WriteRegister(0x3000 ,0x00);
	Camera_WriteRegister(0x3001 ,0x00);
	Camera_WriteRegister(0x3002 ,0x5c);
	Camera_WriteRegister(0x3003 ,0x00);
	Camera_WriteRegister(0x3004 ,0xff);
	Camera_WriteRegister(0x3005 ,0xff);
	Camera_WriteRegister(0x3006 ,0x43);
	Camera_WriteRegister(0x3007 ,0x37);
	Camera_WriteRegister(0x3011 ,0x08);
	Camera_WriteRegister(0x3010 ,0x10);
	Camera_WriteRegister(0x460c ,0x22);
	
	Camera_WriteRegister(0x3815 ,0x01);
	
	Camera_WriteRegister(0x370c ,0xa0);
	Camera_WriteRegister(0x3602 ,0xfc);
	Camera_WriteRegister(0x3612 ,0xff);
	Camera_WriteRegister(0x3634 ,0xc0);
	Camera_WriteRegister(0x3613 ,0x00);
	Camera_WriteRegister(0x3605 ,0x7c);
	Camera_WriteRegister(0x3621 ,0x09);
	Camera_WriteRegister(0x3622 ,0x60);
	Camera_WriteRegister(0x3604 ,0x40);
	Camera_WriteRegister(0x3603 ,0xa7);
	Camera_WriteRegister(0x3603 ,0x27);
	Camera_WriteRegister(0x4000 ,0x21);
	Camera_WriteRegister(0x401d ,0x22);
	Camera_WriteRegister(0x3600 ,0x54);
	Camera_WriteRegister(0x3605 ,0x04);
	Camera_WriteRegister(0x3606 ,0x3f);
	Camera_WriteRegister(0x3c01 ,0x80);
	Camera_WriteRegister(0x5000 ,0x4f);
	Camera_WriteRegister(0x5020 ,0x04);
	Camera_WriteRegister(0x5181 ,0x79);
	Camera_WriteRegister(0x5182 ,0x00);
	Camera_WriteRegister(0x5185 ,0x22);
	Camera_WriteRegister(0x5197 ,0x01);
	
	//Camera_WriteRegister(0x5001 ,0xff);
	Camera_WriteRegister(0x5001 ,0xCF);
	Camera_WriteRegister(0x5002,0x60);
	
	Camera_WriteRegister(0x5500 ,0x0a);
	Camera_WriteRegister(0x5504 ,0x00);
	Camera_WriteRegister(0x5505 ,0x7f);
	Camera_WriteRegister(0x5080 ,0x08);
	Camera_WriteRegister(0x300e ,0x18);
	Camera_WriteRegister(0x4610 ,0x00);
	Camera_WriteRegister(0x471d ,0x05);
	Camera_WriteRegister(0x4708 ,0x06);
	
	//Camera_WriteRegister(0x3808 ,0x03);//WINDOW SIZE HORIZONTAL 1000 * 700
	//Camera_WriteRegister(0x3809 ,0xE8);
	//Camera_WriteRegister(0x380a ,0x02);
	//Camera_WriteRegister(0x380b ,0xBC);

	//Camera_WriteRegister(0x3808 ,0x06);//WINDOW SIZE HORIZONTAL 1600 * 1200
	//Camera_WriteRegister(0x3809 ,0x40);
	//Camera_WriteRegister(0x380a ,0x04);
	//Camera_WriteRegister(0x380b ,0xB0);

	Camera_WriteRegister(0x3808 ,0x02);//WINDOW SIZE HORIZONTAL 640 * 480
	Camera_WriteRegister(0x3809 ,0x80);
	Camera_WriteRegister(0x380a ,0x01);
	Camera_WriteRegister(0x380b ,0xE0);

	//Camera_WriteRegister(0x3808 ,0x04);//WINDOW SIZE HORIZONTAL 1024 * 768
	//Camera_WriteRegister(0x3809 ,0x00);
	//Camera_WriteRegister(0x380a ,0x03);
	//Camera_WriteRegister(0x380b ,0x00);

	
	Camera_WriteRegister(0x380e ,0x07);
	Camera_WriteRegister(0x380f ,0xd0);
	
	Camera_WriteRegister(0x501f ,0x00);
	Camera_WriteRegister(0x5000 ,0x4f);
	Camera_WriteRegister(0x4300 ,0x30);
	Camera_WriteRegister(0x3503 ,0x07);
	Camera_WriteRegister(0x3501 ,0x73);
	Camera_WriteRegister(0x3502 ,0x80);
	Camera_WriteRegister(0x350b ,0x00);
	Camera_WriteRegister(0x3503 ,0x07);
	Camera_WriteRegister(0x3824 ,0x11);
	Camera_WriteRegister(0x3501 ,0x1e);
	Camera_WriteRegister(0x3502 ,0x80);
	Camera_WriteRegister(0x350b ,0x7f);
	
	Camera_WriteRegister(0x380c ,0x0c);//WINDOW TOTAL SIZE
	Camera_WriteRegister(0x380d ,0x80);
	Camera_WriteRegister(0x380e ,0x07);
	Camera_WriteRegister(0x380f ,0xD0);
	
	Camera_WriteRegister(0x3a0d ,0x04);
	Camera_WriteRegister(0x3a0e ,0x03);
	Camera_WriteRegister(0x3818 ,0xc1);
	Camera_WriteRegister(0x3705 ,0xdb);
	Camera_WriteRegister(0x370a ,0x81);
	Camera_WriteRegister(0x3801 ,0x80);
	Camera_WriteRegister(0x3621 ,0x87);
	Camera_WriteRegister(0x3801 ,0x50);
	Camera_WriteRegister(0x3803 ,0x08);
	Camera_WriteRegister(0x3827 ,0x08);
	Camera_WriteRegister(0x3810 ,0x40);

	Camera_WriteRegister(0x3800 ,0x1 );
	Camera_WriteRegister(0x3801 ,0x8A);
	Camera_WriteRegister(0x5680 ,0x01);
	Camera_WriteRegister(0x5681 ,0x8A);

	Camera_WriteRegister(0x3802 ,0x0 );
	Camera_WriteRegister(0x3803 ,0xA );
	Camera_WriteRegister(0x5684 ,0x00);
	Camera_WriteRegister(0x5685 ,0x0A);

	Camera_WriteRegister(0x3804 ,0x06);
	Camera_WriteRegister(0x3805 ,0x40);
	Camera_WriteRegister(0x5682 ,0x06);
	Camera_WriteRegister(0x5683 ,0x40);
	Camera_WriteRegister(0x3806 ,0x04);
	Camera_WriteRegister(0x3807 ,0xc0);
	Camera_WriteRegister(0x5686 ,0x04);
	Camera_WriteRegister(0x5687 ,0xc0);
	
	Camera_WriteRegister(0x3a00 ,0x78);
	Camera_WriteRegister(0x3a1a ,0x05);
	Camera_WriteRegister(0x3a13 ,0x30);
	Camera_WriteRegister(0x3a18 ,0x00);
	Camera_WriteRegister(0x3a19 ,0x7c);
	Camera_WriteRegister(0x3a08 ,0x12);
	Camera_WriteRegister(0x3a09 ,0xc0);
	Camera_WriteRegister(0x3a0a ,0x0f);
	Camera_WriteRegister(0x3a0b ,0xa0);
	Camera_WriteRegister(0x350c ,0x07);
	Camera_WriteRegister(0x350d ,0xd0);
	Camera_WriteRegister(0x3500 ,0x00);
	Camera_WriteRegister(0x3501 ,0x00);
	Camera_WriteRegister(0x3502 ,0x00);
	Camera_WriteRegister(0x350a ,0x00);
	Camera_WriteRegister(0x350b ,0x00);
	Camera_WriteRegister(0x3503 ,0x00);
	Camera_WriteRegister(0x528a ,0x02);
	Camera_WriteRegister(0x528b ,0x04);
	Camera_WriteRegister(0x528c ,0x08);
	Camera_WriteRegister(0x528d ,0x08);
	Camera_WriteRegister(0x528e ,0x08);
	Camera_WriteRegister(0x528f ,0x10);
	Camera_WriteRegister(0x5290 ,0x10);
	Camera_WriteRegister(0x5292 ,0x00);
	Camera_WriteRegister(0x5293 ,0x02);
	Camera_WriteRegister(0x5294 ,0x00);
	Camera_WriteRegister(0x5295 ,0x02);
	Camera_WriteRegister(0x5296 ,0x00);
	Camera_WriteRegister(0x5297 ,0x02);
	Camera_WriteRegister(0x5298 ,0x00);
	Camera_WriteRegister(0x5299 ,0x02);
	Camera_WriteRegister(0x529a ,0x00);
	Camera_WriteRegister(0x529b ,0x02);
	Camera_WriteRegister(0x529c ,0x00);
	Camera_WriteRegister(0x529d ,0x02);
	Camera_WriteRegister(0x529e ,0x00);
	Camera_WriteRegister(0x529f ,0x02);
	Camera_WriteRegister(0x3030 ,0x2b);
	Camera_WriteRegister(0x3a02 ,0x00);
	Camera_WriteRegister(0x3a03 ,0x7d);
	Camera_WriteRegister(0x3a04 ,0x00);
	Camera_WriteRegister(0x3a14 ,0x00);
	Camera_WriteRegister(0x3a15 ,0x7d);
	Camera_WriteRegister(0x3a16 ,0x00);
	Camera_WriteRegister(0x3a00 ,0x78);
	Camera_WriteRegister(0x3a08 ,0x09);
	Camera_WriteRegister(0x3a09 ,0x60);
	Camera_WriteRegister(0x3a0a ,0x07);
	Camera_WriteRegister(0x3a0b ,0xd0);
	Camera_WriteRegister(0x3a0d ,0x08);
	Camera_WriteRegister(0x3a0e ,0x06);
	Camera_WriteRegister(0x5193 ,0x70);
	Camera_WriteRegister(0x589b ,0x04);
	Camera_WriteRegister(0x589a ,0xc5);
	Camera_WriteRegister(0x401e ,0x20);
	Camera_WriteRegister(0x4001 ,0x42);
	Camera_WriteRegister(0x401c ,0x04);
	Camera_WriteRegister(0x528a ,0x01);
	Camera_WriteRegister(0x528b ,0x04);
	Camera_WriteRegister(0x528c ,0x08);
	Camera_WriteRegister(0x528d ,0x10);
	Camera_WriteRegister(0x528e ,0x20);
	Camera_WriteRegister(0x528f ,0x28);
	Camera_WriteRegister(0x5290 ,0x30);
	Camera_WriteRegister(0x5292 ,0x00);
	Camera_WriteRegister(0x5293 ,0x01);
	Camera_WriteRegister(0x5294 ,0x00);
	Camera_WriteRegister(0x5295 ,0x04);
	Camera_WriteRegister(0x5296 ,0x00);
	Camera_WriteRegister(0x5297 ,0x08);
	Camera_WriteRegister(0x5298 ,0x00);
	Camera_WriteRegister(0x5299 ,0x10);
	Camera_WriteRegister(0x529a ,0x00);
	Camera_WriteRegister(0x529b ,0x20);
	Camera_WriteRegister(0x529c ,0x00);
	Camera_WriteRegister(0x529d ,0x28);
	Camera_WriteRegister(0x529e ,0x00);
	Camera_WriteRegister(0x529f ,0x30);
	Camera_WriteRegister(0x5282 ,0x00);
	Camera_WriteRegister(0x5300 ,0x00);
	Camera_WriteRegister(0x5301 ,0x20);
	Camera_WriteRegister(0x5302 ,0x00);
	Camera_WriteRegister(0x5303 ,0x7c);
	Camera_WriteRegister(0x530c ,0x00);
	Camera_WriteRegister(0x530d ,0x0c);
	Camera_WriteRegister(0x530e ,0x20);
	Camera_WriteRegister(0x530f ,0x80);
	Camera_WriteRegister(0x5310 ,0x20);
	Camera_WriteRegister(0x5311 ,0x80);
	Camera_WriteRegister(0x5308 ,0x20);
	Camera_WriteRegister(0x5309 ,0x40);
	Camera_WriteRegister(0x5304 ,0x00);
	Camera_WriteRegister(0x5305 ,0x30);
	Camera_WriteRegister(0x5306 ,0x00);
	Camera_WriteRegister(0x5307 ,0x80);
	Camera_WriteRegister(0x5314 ,0x08);
	Camera_WriteRegister(0x5315 ,0x20);
	Camera_WriteRegister(0x5319 ,0x30);
	Camera_WriteRegister(0x5316 ,0x10);
	Camera_WriteRegister(0x5317 ,0x00);
	Camera_WriteRegister(0x5318 ,0x02);
	Camera_WriteRegister(0x5402 ,0x3f);
	Camera_WriteRegister(0x5403 ,0x00);
	Camera_WriteRegister(0x3406 ,0x00);
	Camera_WriteRegister(0x5180 ,0xff);
	Camera_WriteRegister(0x5181 ,0x52);
	Camera_WriteRegister(0x5182 ,0x11);
	Camera_WriteRegister(0x5183 ,0x14);
	Camera_WriteRegister(0x5184 ,0x25);
	Camera_WriteRegister(0x5185 ,0x24);
	Camera_WriteRegister(0x5186 ,0x06);
	Camera_WriteRegister(0x5187 ,0x08);
	Camera_WriteRegister(0x5188 ,0x08);
	Camera_WriteRegister(0x5189 ,0x7c);
	Camera_WriteRegister(0x518a ,0x60);
	Camera_WriteRegister(0x518b ,0xb2);
	Camera_WriteRegister(0x518c ,0xb2);
	Camera_WriteRegister(0x518d ,0x44);
	Camera_WriteRegister(0x518e ,0x3d);
	Camera_WriteRegister(0x518f ,0x58);
	Camera_WriteRegister(0x5190 ,0x46);
	Camera_WriteRegister(0x5191 ,0xf8);
	Camera_WriteRegister(0x5192 ,0x04);
	Camera_WriteRegister(0x5193 ,0x70);
	Camera_WriteRegister(0x5194 ,0xf0);
	Camera_WriteRegister(0x5195 ,0xf0);
	Camera_WriteRegister(0x5196 ,0x03);
	Camera_WriteRegister(0x5197 ,0x01);
	Camera_WriteRegister(0x5198 ,0x04);
	Camera_WriteRegister(0x5199 ,0x12);
	Camera_WriteRegister(0x519a ,0x04);
	Camera_WriteRegister(0x519b ,0x00);
	Camera_WriteRegister(0x519c ,0x06);
	Camera_WriteRegister(0x519d ,0x82);
	Camera_WriteRegister(0x519e ,0x00);
	Camera_WriteRegister(0x5025 ,0x80);
	Camera_WriteRegister(0x5583 ,0x40);
	Camera_WriteRegister(0x5584 ,0x40);
	Camera_WriteRegister(0x5580 ,0x02);
	Camera_WriteRegister(0x5000 ,0xcf);
	Camera_WriteRegister(0x3710 ,0x10);
	Camera_WriteRegister(0x3632 ,0x51);
	Camera_WriteRegister(0x3702 ,0x10);
	Camera_WriteRegister(0x3703 ,0xb2);
	Camera_WriteRegister(0x3704 ,0x18);
	Camera_WriteRegister(0x370b ,0x40);
	Camera_WriteRegister(0x370d ,0x03);
	Camera_WriteRegister(0x3631 ,0x01);
	Camera_WriteRegister(0x3632 ,0x52);
	Camera_WriteRegister(0x3606 ,0x24);
	Camera_WriteRegister(0x3620 ,0x96);
	Camera_WriteRegister(0x5785 ,0x07);
	Camera_WriteRegister(0x3a13 ,0x30);
	Camera_WriteRegister(0x3600 ,0x52);
	Camera_WriteRegister(0x3604 ,0x48);
	Camera_WriteRegister(0x3606 ,0x1b);
	Camera_WriteRegister(0x370d ,0x0b);
	Camera_WriteRegister(0x370f ,0xc0);
	Camera_WriteRegister(0x3709 ,0x01);
	Camera_WriteRegister(0x3823 ,0x00);
	Camera_WriteRegister(0x5007 ,0x00);
	Camera_WriteRegister(0x5009 ,0x00);
	Camera_WriteRegister(0x5011 ,0x00);
	Camera_WriteRegister(0x5013 ,0x00);
	Camera_WriteRegister(0x519e ,0x00);
	Camera_WriteRegister(0x5086 ,0x00);
	Camera_WriteRegister(0x5087 ,0x00);
	Camera_WriteRegister(0x5088 ,0x00);
	Camera_WriteRegister(0x5089 ,0x00);
	Camera_WriteRegister(0x302b ,0x00);
	Camera_WriteRegister(0x5001 ,0xFF);
	Camera_WriteRegister(0x5583 ,0x50);
	Camera_WriteRegister(0x5584 ,0x50);
	Camera_WriteRegister(0x5580 ,0x02);
	Camera_WriteRegister(0x3c01 ,0x80);
	Camera_WriteRegister(0x3c00 ,0x04);
	//LENS
	Camera_WriteRegister(0x5800 ,0x48);
	Camera_WriteRegister(0x5801 ,0x31);
	Camera_WriteRegister(0x5802 ,0x21);
	Camera_WriteRegister(0x5803 ,0x1b);
	Camera_WriteRegister(0x5804 ,0x1a);
	Camera_WriteRegister(0x5805 ,0x1e);
	Camera_WriteRegister(0x5806 ,0x29);
	Camera_WriteRegister(0x5807 ,0x38);
	Camera_WriteRegister(0x5808 ,0x26);
	Camera_WriteRegister(0x5809 ,0x17);
	Camera_WriteRegister(0x580a ,0x11);
	Camera_WriteRegister(0x580b ,0xe );
	Camera_WriteRegister(0x580c ,0xd );
	Camera_WriteRegister(0x580d ,0xe );
	Camera_WriteRegister(0x580e ,0x13);
	Camera_WriteRegister(0x580f ,0x1a);
	Camera_WriteRegister(0x5810 ,0x15);
	Camera_WriteRegister(0x5811 ,0xd );
	Camera_WriteRegister(0x5812 ,0x8 );
	Camera_WriteRegister(0x5813 ,0x5 );
	Camera_WriteRegister(0x5814 ,0x4 );
	Camera_WriteRegister(0x5815 ,0x5 );
	Camera_WriteRegister(0x5816 ,0x9 );
	Camera_WriteRegister(0x5817 ,0xd );
	Camera_WriteRegister(0x5818 ,0x11);
	Camera_WriteRegister(0x5819 ,0xa );
	Camera_WriteRegister(0x581a ,0x4 );
	Camera_WriteRegister(0x581b ,0x0 );
	Camera_WriteRegister(0x581c ,0x0 );
	Camera_WriteRegister(0x581d ,0x1 );
	Camera_WriteRegister(0x581e ,0x6 );
	Camera_WriteRegister(0x581f ,0x9 );
	Camera_WriteRegister(0x5820 ,0x12);
	Camera_WriteRegister(0x5821 ,0xb );
	Camera_WriteRegister(0x5822 ,0x4 );
	Camera_WriteRegister(0x5823 ,0x0 );
	Camera_WriteRegister(0x5824 ,0x0 );
	Camera_WriteRegister(0x5825 ,0x1 );
	Camera_WriteRegister(0x5826 ,0x6 );
	Camera_WriteRegister(0x5827 ,0xa );
	Camera_WriteRegister(0x5828 ,0x17);
	Camera_WriteRegister(0x5829 ,0xf );
	Camera_WriteRegister(0x582a ,0x9 );
	Camera_WriteRegister(0x582b ,0x6 );
	Camera_WriteRegister(0x582c ,0x5 );
	Camera_WriteRegister(0x582d ,0x6 );
	Camera_WriteRegister(0x582e ,0xa );
	Camera_WriteRegister(0x582f ,0xe );
	Camera_WriteRegister(0x5830 ,0x28);
	Camera_WriteRegister(0x5831 ,0x1a);
	Camera_WriteRegister(0x5832 ,0x11);
	Camera_WriteRegister(0x5833 ,0xe );
	Camera_WriteRegister(0x5834 ,0xe );
	Camera_WriteRegister(0x5835 ,0xf );
	Camera_WriteRegister(0x5836 ,0x15);
	Camera_WriteRegister(0x5837 ,0x1d);
	Camera_WriteRegister(0x5838 ,0x6e);
	Camera_WriteRegister(0x5839 ,0x39);
	Camera_WriteRegister(0x583a ,0x27);
	Camera_WriteRegister(0x583b ,0x1f);
	Camera_WriteRegister(0x583c ,0x1e);
	Camera_WriteRegister(0x583d ,0x23);
	Camera_WriteRegister(0x583e ,0x2f);
	Camera_WriteRegister(0x583f ,0x41);
	Camera_WriteRegister(0x5840 ,0xe );
	Camera_WriteRegister(0x5841 ,0xc );
	Camera_WriteRegister(0x5842 ,0xd );
	Camera_WriteRegister(0x5843 ,0xc );
	Camera_WriteRegister(0x5844 ,0xc );
	Camera_WriteRegister(0x5845 ,0xc );
	Camera_WriteRegister(0x5846 ,0xc );
	Camera_WriteRegister(0x5847 ,0xc );
	Camera_WriteRegister(0x5848 ,0xd );
	Camera_WriteRegister(0x5849 ,0xe );
	Camera_WriteRegister(0x584a ,0xe );
	Camera_WriteRegister(0x584b ,0xa );
	Camera_WriteRegister(0x584c ,0xe );
	Camera_WriteRegister(0x584d ,0xe );
	Camera_WriteRegister(0x584e ,0x10);
	Camera_WriteRegister(0x584f ,0x10);
	Camera_WriteRegister(0x5850 ,0x11);
	Camera_WriteRegister(0x5851 ,0xa );
	Camera_WriteRegister(0x5852 ,0xf );
	Camera_WriteRegister(0x5853 ,0xe );
	Camera_WriteRegister(0x5854 ,0x10);
	Camera_WriteRegister(0x5855 ,0x10);
	Camera_WriteRegister(0x5856 ,0x10);
	Camera_WriteRegister(0x5857 ,0xa );
	Camera_WriteRegister(0x5858 ,0xe );
	Camera_WriteRegister(0x5859 ,0xe );
	Camera_WriteRegister(0x585a ,0xf );
	Camera_WriteRegister(0x585b ,0xf );
	Camera_WriteRegister(0x585c ,0xf );
	Camera_WriteRegister(0x585d ,0xa );
	Camera_WriteRegister(0x585e ,0x9 );
	Camera_WriteRegister(0x585f ,0xd );
	Camera_WriteRegister(0x5860 ,0xc );
	Camera_WriteRegister(0x5861 ,0xb );
	Camera_WriteRegister(0x5862 ,0xd );
	Camera_WriteRegister(0x5863 ,0x7 );
	Camera_WriteRegister(0x5864 ,0x17);
	Camera_WriteRegister(0x5865 ,0x14);
	Camera_WriteRegister(0x5866 ,0x18);
	Camera_WriteRegister(0x5867 ,0x18);
	Camera_WriteRegister(0x5868 ,0x16);
	Camera_WriteRegister(0x5869 ,0x12);
	Camera_WriteRegister(0x586a ,0x1b);
	Camera_WriteRegister(0x586b ,0x1a);
	Camera_WriteRegister(0x586c ,0x16);
	Camera_WriteRegister(0x586d ,0x16);
	Camera_WriteRegister(0x586e ,0x18);
	Camera_WriteRegister(0x586f ,0x1f);
	Camera_WriteRegister(0x5870 ,0x1c);
	Camera_WriteRegister(0x5871 ,0x16);
	Camera_WriteRegister(0x5872 ,0x10);
	Camera_WriteRegister(0x5873 ,0xf );
	Camera_WriteRegister(0x5874 ,0x13);
	Camera_WriteRegister(0x5875 ,0x1c);
	Camera_WriteRegister(0x5876 ,0x1e);
	Camera_WriteRegister(0x5877 ,0x17);
	Camera_WriteRegister(0x5878 ,0x11);
	Camera_WriteRegister(0x5879 ,0x11);
	Camera_WriteRegister(0x587a ,0x14);
	Camera_WriteRegister(0x587b ,0x1e);
	Camera_WriteRegister(0x587c ,0x1c);
	Camera_WriteRegister(0x587d ,0x1c);
	Camera_WriteRegister(0x587e ,0x1a);
	Camera_WriteRegister(0x587f ,0x1a);
	Camera_WriteRegister(0x5880 ,0x1b);
	Camera_WriteRegister(0x5881 ,0x1f);
	Camera_WriteRegister(0x5882 ,0x14);
	Camera_WriteRegister(0x5883 ,0x1a);
	Camera_WriteRegister(0x5884 ,0x1d);
	Camera_WriteRegister(0x5885 ,0x1e);
	Camera_WriteRegister(0x5886 ,0x1a);
	Camera_WriteRegister(0x5887 ,0x1a);
	//AWB
	Camera_WriteRegister(0x5180 ,0xff);
	Camera_WriteRegister(0x5181 ,0x52);
	Camera_WriteRegister(0x5182 ,0x11);
	Camera_WriteRegister(0x5183 ,0x14);
	Camera_WriteRegister(0x5184 ,0x25);
	Camera_WriteRegister(0x5185 ,0x24);
	Camera_WriteRegister(0x5186 ,0x14);
	Camera_WriteRegister(0x5187 ,0x14);
	Camera_WriteRegister(0x5188 ,0x14);
	Camera_WriteRegister(0x5189 ,0x69);
	Camera_WriteRegister(0x518a ,0x60);
	Camera_WriteRegister(0x518b ,0xa2);
	Camera_WriteRegister(0x518c ,0x9c);
	Camera_WriteRegister(0x518d ,0x36);
	Camera_WriteRegister(0x518e ,0x34);
	Camera_WriteRegister(0x518f ,0x54);
	Camera_WriteRegister(0x5190 ,0x4c);
	Camera_WriteRegister(0x5191 ,0xf8);
	Camera_WriteRegister(0x5192 ,0x04);
	Camera_WriteRegister(0x5193 ,0x70);
	Camera_WriteRegister(0x5194 ,0xf0);
	Camera_WriteRegister(0x5195 ,0xf0);
	Camera_WriteRegister(0x5196 ,0x03);
	Camera_WriteRegister(0x5197 ,0x01);
	Camera_WriteRegister(0x5198 ,0x05);
	Camera_WriteRegister(0x5199 ,0x2f);
	Camera_WriteRegister(0x519a ,0x04);
	Camera_WriteRegister(0x519b ,0x00);
	Camera_WriteRegister(0x519c ,0x06);
	Camera_WriteRegister(0x519d ,0xa0);
	Camera_WriteRegister(0x519e ,0xa0);
	//D/S
	Camera_WriteRegister(0x528a ,0x00);
	Camera_WriteRegister(0x528b ,0x01);
	Camera_WriteRegister(0x528c ,0x04);
	Camera_WriteRegister(0x528d ,0x08);
	Camera_WriteRegister(0x528e ,0x10);
	Camera_WriteRegister(0x528f ,0x20);
	Camera_WriteRegister(0x5290 ,0x30);
	Camera_WriteRegister(0x5292 ,0x00);
	Camera_WriteRegister(0x5293 ,0x00);
	Camera_WriteRegister(0x5294 ,0x00);
	Camera_WriteRegister(0x5295 ,0x01);
	Camera_WriteRegister(0x5296 ,0x00);
	Camera_WriteRegister(0x5297 ,0x04);
	Camera_WriteRegister(0x5298 ,0x00);
	Camera_WriteRegister(0x5299 ,0x08);
	Camera_WriteRegister(0x529a ,0x00);
	Camera_WriteRegister(0x529b ,0x10);
	Camera_WriteRegister(0x529c ,0x00);
	Camera_WriteRegister(0x529d ,0x20);
	Camera_WriteRegister(0x529e ,0x00);
	Camera_WriteRegister(0x529f ,0x30);
	Camera_WriteRegister(0x5282 ,0x00);
	Camera_WriteRegister(0x5300 ,0x00);
	Camera_WriteRegister(0x5301 ,0x20);
	Camera_WriteRegister(0x5302 ,0x00);
	Camera_WriteRegister(0x5303 ,0x7c);
	Camera_WriteRegister(0x530c ,0x00);
	Camera_WriteRegister(0x530d ,0x10);
	Camera_WriteRegister(0x530e ,0x20);
	Camera_WriteRegister(0x530f ,0x80);
	Camera_WriteRegister(0x5310 ,0x20);
	Camera_WriteRegister(0x5311 ,0x80);
	Camera_WriteRegister(0x5308 ,0x20);
	Camera_WriteRegister(0x5309 ,0x40);
	Camera_WriteRegister(0x5304 ,0x00);
	Camera_WriteRegister(0x5305 ,0x30);
	Camera_WriteRegister(0x5306 ,0x00);
	Camera_WriteRegister(0x5307 ,0x80);
	Camera_WriteRegister(0x5314 ,0x08);
	Camera_WriteRegister(0x5315 ,0x20);
	Camera_WriteRegister(0x5319 ,0x30);
	Camera_WriteRegister(0x5316 ,0x10);
	Camera_WriteRegister(0x5317 ,0x00);
	Camera_WriteRegister(0x5318 ,0x02);
	//CMX
	Camera_WriteRegister(0x5380 ,0x01);
	Camera_WriteRegister(0x5381 ,0x00);
	Camera_WriteRegister(0x5382 ,0x00);
	Camera_WriteRegister(0x5383 ,0x1f);
	Camera_WriteRegister(0x5384 ,0x00);
	Camera_WriteRegister(0x5385 ,0x06);
	Camera_WriteRegister(0x5386 ,0x00);
	Camera_WriteRegister(0x5387 ,0x00);
	Camera_WriteRegister(0x5388 ,0x00);
	Camera_WriteRegister(0x5389 ,0xE1);
	Camera_WriteRegister(0x538A ,0x00);
	Camera_WriteRegister(0x538B ,0x2B);
	Camera_WriteRegister(0x538C ,0x00);
	Camera_WriteRegister(0x538D ,0x00);
	Camera_WriteRegister(0x538E ,0x00);
	Camera_WriteRegister(0x538F ,0x10);
	Camera_WriteRegister(0x5390 ,0x00);
	Camera_WriteRegister(0x5391 ,0xB3);
	Camera_WriteRegister(0x5392 ,0x00);
	Camera_WriteRegister(0x5393 ,0xA6);
	Camera_WriteRegister(0x5394 ,0x08);
	//GAMMA
	Camera_WriteRegister(0x5480 ,0x0c);
	Camera_WriteRegister(0x5481 ,0x18);
	Camera_WriteRegister(0x5482 ,0x2f);
	Camera_WriteRegister(0x5483 ,0x55);
	Camera_WriteRegister(0x5484 ,0x64);
	Camera_WriteRegister(0x5485 ,0x71);
	Camera_WriteRegister(0x5486 ,0x7d);
	Camera_WriteRegister(0x5487 ,0x87);
	Camera_WriteRegister(0x5488 ,0x91);
	Camera_WriteRegister(0x5489 ,0x9a);
	Camera_WriteRegister(0x548A ,0xaa);
	Camera_WriteRegister(0x548B ,0xb8);
	Camera_WriteRegister(0x548C ,0xcd);
	Camera_WriteRegister(0x548D ,0xdd);
	Camera_WriteRegister(0x548E ,0xea);
	Camera_WriteRegister(0x548F ,0x1d);
	Camera_WriteRegister(0x5490 ,0x05);
	Camera_WriteRegister(0x5491 ,0x00);
	Camera_WriteRegister(0x5492 ,0x04);
	Camera_WriteRegister(0x5493 ,0x20);
	Camera_WriteRegister(0x5494 ,0x03);
	Camera_WriteRegister(0x5495 ,0x60);
	Camera_WriteRegister(0x5496 ,0x02);
	Camera_WriteRegister(0x5497 ,0xB8);
	Camera_WriteRegister(0x5498 ,0x02);
	Camera_WriteRegister(0x5499 ,0x86);
	Camera_WriteRegister(0x549A ,0x02);
	Camera_WriteRegister(0x549B ,0x5B);
	Camera_WriteRegister(0x549C ,0x02);
	Camera_WriteRegister(0x549D ,0x3B);
	Camera_WriteRegister(0x549E ,0x02);
	Camera_WriteRegister(0x549F ,0x1C);
	Camera_WriteRegister(0x54A0 ,0x02);
	Camera_WriteRegister(0x54A1 ,0x04);
	Camera_WriteRegister(0x54A2 ,0x01);
	Camera_WriteRegister(0x54A3 ,0xED);
	Camera_WriteRegister(0x54A4 ,0x01);
	Camera_WriteRegister(0x54A5 ,0xC5);
	Camera_WriteRegister(0x54A6 ,0x01);
	Camera_WriteRegister(0x54A7 ,0xA5);
	Camera_WriteRegister(0x54A8 ,0x01);
	Camera_WriteRegister(0x54A9 ,0x6C);
	Camera_WriteRegister(0x54AA ,0x01);
	Camera_WriteRegister(0x54AB ,0x41);
	Camera_WriteRegister(0x54AC ,0x01);
	Camera_WriteRegister(0x54AD ,0x20);
	Camera_WriteRegister(0x54AE ,0x00);
	Camera_WriteRegister(0x54AF ,0x16);
	Camera_WriteRegister(0x54B0 ,0x01);
	Camera_WriteRegister(0x54B1 ,0x20);
	Camera_WriteRegister(0x54B2 ,0x00);
	Camera_WriteRegister(0x54B3 ,0x10);
	Camera_WriteRegister(0x54B4 ,0x00);
	Camera_WriteRegister(0x54B5 ,0xf0);
	Camera_WriteRegister(0x54B6 ,0x00);
	Camera_WriteRegister(0x54B7 ,0xDF);
	Camera_WriteRegister(0x5402 ,0x3f);
	Camera_WriteRegister(0x5403 ,0x00);
	//UV ADJUST
	Camera_WriteRegister(0x5500 ,0x10);
	Camera_WriteRegister(0x5502 ,0x00);
	Camera_WriteRegister(0x5503 ,0x06);
	Camera_WriteRegister(0x5504 ,0x00);
	Camera_WriteRegister(0x5505 ,0x7f);
	//AE
	Camera_WriteRegister(0x5025 ,0x80);
	Camera_WriteRegister(0x3a0f ,0x30);
	Camera_WriteRegister(0x3a10 ,0x28);
	Camera_WriteRegister(0x3a1b ,0x30);
	Camera_WriteRegister(0x3a1e ,0x28);
	Camera_WriteRegister(0x3a11 ,0x61);
	Camera_WriteRegister(0x3a1f ,0x10);
	Camera_WriteRegister(0x5688 ,0xfd);
	Camera_WriteRegister(0x5689 ,0xdf);
	Camera_WriteRegister(0x568a ,0xfe);
	Camera_WriteRegister(0x568b ,0xef);
	Camera_WriteRegister(0x568c ,0xfe);
	Camera_WriteRegister(0x568d ,0xef);
	Camera_WriteRegister(0x568e ,0xaa);
	Camera_WriteRegister(0x568f ,0xaa);

	Camera_WriteRegister(0x4300 ,0x32);//YUV 422, UYVY 설정
	Camera_WriteRegister(0x4730,0x01);//CCIR656 MODE ENABLE
	
}

void QVGA_320_240_PreView( void )
{
	Camera_WriteRegister(0x3103 ,0x93);
	Camera_WriteRegister(0x3008 ,0x82);
	Camera_WriteRegister(0x3017 ,0x7f);
	Camera_WriteRegister(0x3018 ,0xfc);
	Camera_WriteRegister(0x3810 ,0xc2);
	Camera_WriteRegister(0x3615 ,0xf0);
	Camera_WriteRegister(0x3000 ,0x00);
	Camera_WriteRegister(0x3001 ,0x00);
	Camera_WriteRegister(0x3002 ,0x5c);
	Camera_WriteRegister(0x3003 ,0x00);
	Camera_WriteRegister(0x3004 ,0xff);
	Camera_WriteRegister(0x3005 ,0xff);
	Camera_WriteRegister(0x3006 ,0x43);
	Camera_WriteRegister(0x3007 ,0x37);
	Camera_WriteRegister(0x3011 ,0x08);
	Camera_WriteRegister(0x3010 ,0x10);
	Camera_WriteRegister(0x460c ,0x22);
	Camera_WriteRegister(0x3815 ,0x04);
	Camera_WriteRegister(0x370c ,0xa0);
	Camera_WriteRegister(0x3602 ,0xfc);
	Camera_WriteRegister(0x3612 ,0xff);
	Camera_WriteRegister(0x3634 ,0xc0);
	Camera_WriteRegister(0x3613 ,0x00);
	Camera_WriteRegister(0x3605 ,0x7c);
	Camera_WriteRegister(0x3621 ,0x09);
	Camera_WriteRegister(0x3622 ,0x60);
	Camera_WriteRegister(0x3604 ,0x40);
	Camera_WriteRegister(0x3603 ,0xa7);
	Camera_WriteRegister(0x3603 ,0x27);
	Camera_WriteRegister(0x4000 ,0x21);
	Camera_WriteRegister(0x401d ,0x22);
	Camera_WriteRegister(0x3600 ,0x54);
	Camera_WriteRegister(0x3605 ,0x04);
	Camera_WriteRegister(0x3606 ,0x3f);
	Camera_WriteRegister(0x3c01 ,0x80);
	Camera_WriteRegister(0x5000 ,0x4f);
	Camera_WriteRegister(0x5020 ,0x04);
	Camera_WriteRegister(0x5181 ,0x79);
	Camera_WriteRegister(0x5182 ,0x00);
	Camera_WriteRegister(0x5185 ,0x22);
	Camera_WriteRegister(0x5197 ,0x01);
	Camera_WriteRegister(0x5001 ,0xff);
	Camera_WriteRegister(0x5500 ,0x0a);
	Camera_WriteRegister(0x5504 ,0x00);
	Camera_WriteRegister(0x5505 ,0x7f);
	Camera_WriteRegister(0x5080 ,0x08);
	Camera_WriteRegister(0x300e ,0x18);
	Camera_WriteRegister(0x4610 ,0x00);
	Camera_WriteRegister(0x471d ,0x05);
	Camera_WriteRegister(0x4708 ,0x06);
	Camera_WriteRegister(0x3808 ,0x02);
	Camera_WriteRegister(0x3809 ,0x80);
	Camera_WriteRegister(0x380a ,0x01);
	Camera_WriteRegister(0x380b ,0xe0);
	Camera_WriteRegister(0x380e ,0x07);
	Camera_WriteRegister(0x380f ,0xd0);
	Camera_WriteRegister(0x501f ,0x00);
	Camera_WriteRegister(0x5000 ,0x4f);
	Camera_WriteRegister(0x4300 ,0x32);
	Camera_WriteRegister(0x3503 ,0x07);
	Camera_WriteRegister(0x3501 ,0x73);
	Camera_WriteRegister(0x3502 ,0x80);
	Camera_WriteRegister(0x350b ,0x00);
	Camera_WriteRegister(0x3503 ,0x07);
	Camera_WriteRegister(0x3824 ,0x11);
	Camera_WriteRegister(0x3501 ,0x1e);
	Camera_WriteRegister(0x3502 ,0x80);
	Camera_WriteRegister(0x350b ,0x7f);
	Camera_WriteRegister(0x380c ,0x0c);
	Camera_WriteRegister(0x380d ,0x80);
	Camera_WriteRegister(0x380e ,0x03);
	Camera_WriteRegister(0x380f ,0xe8);
	Camera_WriteRegister(0x3a0d ,0x04);
	Camera_WriteRegister(0x3a0e ,0x03);
	Camera_WriteRegister(0x3818 ,0xc1);
	Camera_WriteRegister(0x3705 ,0xdb);
	Camera_WriteRegister(0x370a ,0x81);
	Camera_WriteRegister(0x3801 ,0x80);
	Camera_WriteRegister(0x3621 ,0x87);
	Camera_WriteRegister(0x3801 ,0x50);
	Camera_WriteRegister(0x3803 ,0x08);
	Camera_WriteRegister(0x3827 ,0x08);
	Camera_WriteRegister(0x3810 ,0x40);
	Camera_WriteRegister(0x3804 ,0x05);
	Camera_WriteRegister(0x3805 ,0x00);
	Camera_WriteRegister(0x5682 ,0x05);
	Camera_WriteRegister(0x5683 ,0x00);
	Camera_WriteRegister(0x3806 ,0x03);
	Camera_WriteRegister(0x3807 ,0xc0);
	Camera_WriteRegister(0x5686 ,0x03);
	Camera_WriteRegister(0x5687 ,0xbc);
	Camera_WriteRegister(0x3a00 ,0x78);
	Camera_WriteRegister(0x3a1a ,0x05);
	Camera_WriteRegister(0x3a13 ,0x30);
	Camera_WriteRegister(0x3a18 ,0x00);
	Camera_WriteRegister(0x3a19 ,0x7c);
	Camera_WriteRegister(0x3a08 ,0x12);
	Camera_WriteRegister(0x3a09 ,0xc0);
	Camera_WriteRegister(0x3a0a ,0x0f);
	Camera_WriteRegister(0x3a0b ,0xa0);
	Camera_WriteRegister(0x350c ,0x07);
	Camera_WriteRegister(0x350d ,0xd0);
	Camera_WriteRegister(0x3500 ,0x00);
	Camera_WriteRegister(0x3501 ,0x00);
	Camera_WriteRegister(0x3502 ,0x00);
	Camera_WriteRegister(0x350a ,0x00);
	Camera_WriteRegister(0x350b ,0x00);
	Camera_WriteRegister(0x3503 ,0x00);
	Camera_WriteRegister(0x528a ,0x02);
	Camera_WriteRegister(0x528b ,0x04);
	Camera_WriteRegister(0x528c ,0x08);
	Camera_WriteRegister(0x528d ,0x08);
	Camera_WriteRegister(0x528e ,0x08);
	Camera_WriteRegister(0x528f ,0x10);
	Camera_WriteRegister(0x5290 ,0x10);
	Camera_WriteRegister(0x5292 ,0x00);
	Camera_WriteRegister(0x5293 ,0x02);
	Camera_WriteRegister(0x5294 ,0x00);
	Camera_WriteRegister(0x5295 ,0x02);
	Camera_WriteRegister(0x5296 ,0x00);
	Camera_WriteRegister(0x5297 ,0x02);
	Camera_WriteRegister(0x5298 ,0x00);
	Camera_WriteRegister(0x5299 ,0x02);
	Camera_WriteRegister(0x529a ,0x00);
	Camera_WriteRegister(0x529b ,0x02);
	Camera_WriteRegister(0x529c ,0x00);
	Camera_WriteRegister(0x529d ,0x02);
	Camera_WriteRegister(0x529e ,0x00);
	Camera_WriteRegister(0x529f ,0x02);
	Camera_WriteRegister(0x3030 ,0x2b);
	Camera_WriteRegister(0x3a02 ,0x00);
	Camera_WriteRegister(0x3a03 ,0x7d);
	Camera_WriteRegister(0x3a04 ,0x00);
	Camera_WriteRegister(0x3a14 ,0x00);
	Camera_WriteRegister(0x3a15 ,0x7d);
	Camera_WriteRegister(0x3a16 ,0x00);
	Camera_WriteRegister(0x3a00 ,0x78);
	Camera_WriteRegister(0x3a08 ,0x09);
	Camera_WriteRegister(0x3a09 ,0x60);
	Camera_WriteRegister(0x3a0a ,0x07);
	Camera_WriteRegister(0x3a0b ,0xd0);
	Camera_WriteRegister(0x3a0d ,0x08);
	Camera_WriteRegister(0x3a0e ,0x06);
	Camera_WriteRegister(0x5193 ,0x70);
	Camera_WriteRegister(0x589b ,0x04);
	Camera_WriteRegister(0x589a ,0xc5);
	Camera_WriteRegister(0x401e ,0x20);
	Camera_WriteRegister(0x4001 ,0x42);
	Camera_WriteRegister(0x401c ,0x04);
	Camera_WriteRegister(0x528a ,0x01);
	Camera_WriteRegister(0x528b ,0x04);
	Camera_WriteRegister(0x528c ,0x08);
	Camera_WriteRegister(0x528d ,0x10);
	Camera_WriteRegister(0x528e ,0x20);
	Camera_WriteRegister(0x528f ,0x28);
	Camera_WriteRegister(0x5290 ,0x30);
	Camera_WriteRegister(0x5292 ,0x00);
	Camera_WriteRegister(0x5293 ,0x01);
	Camera_WriteRegister(0x5294 ,0x00);
	Camera_WriteRegister(0x5295 ,0x04);
	Camera_WriteRegister(0x5296 ,0x00);
	Camera_WriteRegister(0x5297 ,0x08);
	Camera_WriteRegister(0x5298 ,0x00);
	Camera_WriteRegister(0x5299 ,0x10);
	Camera_WriteRegister(0x529a ,0x00);
	Camera_WriteRegister(0x529b ,0x20);
	Camera_WriteRegister(0x529c ,0x00);
	Camera_WriteRegister(0x529d ,0x28);
	Camera_WriteRegister(0x529e ,0x00);
	Camera_WriteRegister(0x529f ,0x30);
	Camera_WriteRegister(0x5282 ,0x00);
	Camera_WriteRegister(0x5300 ,0x00);
	Camera_WriteRegister(0x5301 ,0x20);
	Camera_WriteRegister(0x5302 ,0x00);
	Camera_WriteRegister(0x5303 ,0x7c);
	Camera_WriteRegister(0x530c ,0x00);
	Camera_WriteRegister(0x530d ,0x0c);
	Camera_WriteRegister(0x530e ,0x20);
	Camera_WriteRegister(0x530f ,0x80);
	Camera_WriteRegister(0x5310 ,0x20);
	Camera_WriteRegister(0x5311 ,0x80);
	Camera_WriteRegister(0x5308 ,0x20);
	Camera_WriteRegister(0x5309 ,0x40);
	Camera_WriteRegister(0x5304 ,0x00);
	Camera_WriteRegister(0x5305 ,0x30);
	Camera_WriteRegister(0x5306 ,0x00);
	Camera_WriteRegister(0x5307 ,0x80);
	Camera_WriteRegister(0x5314 ,0x08);
	Camera_WriteRegister(0x5315 ,0x20);
	Camera_WriteRegister(0x5319 ,0x30);
	Camera_WriteRegister(0x5316 ,0x10);
	Camera_WriteRegister(0x5317 ,0x00);
	Camera_WriteRegister(0x5318 ,0x02);
	Camera_WriteRegister(0x5402 ,0x3f);
	Camera_WriteRegister(0x5403 ,0x00);
	Camera_WriteRegister(0x3406 ,0x00);
	Camera_WriteRegister(0x5180 ,0xff);
	Camera_WriteRegister(0x5181 ,0x52);
	Camera_WriteRegister(0x5182 ,0x11);
	Camera_WriteRegister(0x5183 ,0x14);
	Camera_WriteRegister(0x5184 ,0x25);
	Camera_WriteRegister(0x5185 ,0x24);
	Camera_WriteRegister(0x5186 ,0x06);
	Camera_WriteRegister(0x5187 ,0x08);
	Camera_WriteRegister(0x5188 ,0x08);
	Camera_WriteRegister(0x5189 ,0x7c);
	Camera_WriteRegister(0x518a ,0x60);
	Camera_WriteRegister(0x518b ,0xb2);
	Camera_WriteRegister(0x518c ,0xb2);
	Camera_WriteRegister(0x518d ,0x44);
	Camera_WriteRegister(0x518e ,0x3d);
	Camera_WriteRegister(0x518f ,0x58);
	Camera_WriteRegister(0x5190 ,0x46);
	Camera_WriteRegister(0x5191 ,0xf8);
	Camera_WriteRegister(0x5192 ,0x04);
	Camera_WriteRegister(0x5193 ,0x70);
	Camera_WriteRegister(0x5194 ,0xf0);
	Camera_WriteRegister(0x5195 ,0xf0);
	Camera_WriteRegister(0x5196 ,0x03);
	Camera_WriteRegister(0x5197 ,0x01);
	Camera_WriteRegister(0x5198 ,0x04);
	Camera_WriteRegister(0x5199 ,0x12);
	Camera_WriteRegister(0x519a ,0x04);
	Camera_WriteRegister(0x519b ,0x00);
	Camera_WriteRegister(0x519c ,0x06);
	Camera_WriteRegister(0x519d ,0x82);
	Camera_WriteRegister(0x519e ,0x00);
	Camera_WriteRegister(0x5025 ,0x80);
	Camera_WriteRegister(0x5583 ,0x40);
	Camera_WriteRegister(0x5584 ,0x40);
	Camera_WriteRegister(0x5580 ,0x02);
	Camera_WriteRegister(0x5000 ,0xcf);
	Camera_WriteRegister(0x3710 ,0x10);
	Camera_WriteRegister(0x3632 ,0x51);
	Camera_WriteRegister(0x3702 ,0x10);
	Camera_WriteRegister(0x3703 ,0xb2);
	Camera_WriteRegister(0x3704 ,0x18);
	Camera_WriteRegister(0x370b ,0x40);
	Camera_WriteRegister(0x370d ,0x03);
	Camera_WriteRegister(0x3631 ,0x01);
	Camera_WriteRegister(0x3632 ,0x52);
	Camera_WriteRegister(0x3606 ,0x24);
	Camera_WriteRegister(0x3620 ,0x96);
	Camera_WriteRegister(0x5785 ,0x07);
	Camera_WriteRegister(0x3a13 ,0x30);
	Camera_WriteRegister(0x3600 ,0x52);
	Camera_WriteRegister(0x3604 ,0x48);
	Camera_WriteRegister(0x3606 ,0x1b);
	Camera_WriteRegister(0x370d ,0x0b);
	Camera_WriteRegister(0x370f ,0xc0);
	Camera_WriteRegister(0x3709 ,0x01);
	Camera_WriteRegister(0x3823 ,0x00);
	Camera_WriteRegister(0x5007 ,0x00);
	Camera_WriteRegister(0x5009 ,0x00);
	Camera_WriteRegister(0x5011 ,0x00);
	Camera_WriteRegister(0x5013 ,0x00);
	Camera_WriteRegister(0x519e ,0x00);
	Camera_WriteRegister(0x5086 ,0x00);
	Camera_WriteRegister(0x5087 ,0x00);
	Camera_WriteRegister(0x5088 ,0x00);
	Camera_WriteRegister(0x5089 ,0x00);
	Camera_WriteRegister(0x302b ,0x00);
	Camera_WriteRegister(0x3808 ,0x01);
	Camera_WriteRegister(0x3809 ,0x40);
	Camera_WriteRegister(0x380a ,0x00);
	Camera_WriteRegister(0x380b ,0xf0);
	Camera_WriteRegister(0x3a00 ,0x78);
	Camera_WriteRegister(0x5001 ,0xFF);
	Camera_WriteRegister(0x5583 ,0x50);
	Camera_WriteRegister(0x5584 ,0x50);
	Camera_WriteRegister(0x5580 ,0x02);
	Camera_WriteRegister(0x3c01 ,0x80);
	Camera_WriteRegister(0x3c00 ,0x04);


//LENS
	Camera_WriteRegister(0x5800 ,0x48);
	Camera_WriteRegister(0x5801 ,0x31);
	Camera_WriteRegister(0x5802 ,0x21);
	Camera_WriteRegister(0x5803 ,0x1b);
	Camera_WriteRegister(0x5804 ,0x1a);
	Camera_WriteRegister(0x5805 ,0x1e);
	Camera_WriteRegister(0x5806 ,0x29);
	Camera_WriteRegister(0x5807 ,0x38);
	Camera_WriteRegister(0x5808 ,0x26);
	Camera_WriteRegister(0x5809 ,0x17);
	Camera_WriteRegister(0x580a ,0x11);
	Camera_WriteRegister(0x580b ,0xe );
	Camera_WriteRegister(0x580c ,0xd );
	Camera_WriteRegister(0x580d ,0xe );
	Camera_WriteRegister(0x580e ,0x13);
	Camera_WriteRegister(0x580f ,0x1a);
	Camera_WriteRegister(0x5810 ,0x15);
	Camera_WriteRegister(0x5811 ,0xd );
	Camera_WriteRegister(0x5812 ,0x8 );
	Camera_WriteRegister(0x5813 ,0x5 );
	Camera_WriteRegister(0x5814 ,0x4 );
	Camera_WriteRegister(0x5815 ,0x5 );
	Camera_WriteRegister(0x5816 ,0x9 );
	Camera_WriteRegister(0x5817 ,0xd );
	Camera_WriteRegister(0x5818 ,0x11);
	Camera_WriteRegister(0x5819 ,0xa );
	Camera_WriteRegister(0x581a ,0x4 );
	Camera_WriteRegister(0x581b ,0x0 );
	Camera_WriteRegister(0x581c ,0x0 );
	Camera_WriteRegister(0x581d ,0x1 );
	Camera_WriteRegister(0x581e ,0x6 );
	Camera_WriteRegister(0x581f ,0x9 );
	Camera_WriteRegister(0x5820 ,0x12);
	Camera_WriteRegister(0x5821 ,0xb );
	Camera_WriteRegister(0x5822 ,0x4 );
	Camera_WriteRegister(0x5823 ,0x0 );
	Camera_WriteRegister(0x5824 ,0x0 );
	Camera_WriteRegister(0x5825 ,0x1 );
	Camera_WriteRegister(0x5826 ,0x6 );
	Camera_WriteRegister(0x5827 ,0xa );
	Camera_WriteRegister(0x5828 ,0x17);
	Camera_WriteRegister(0x5829 ,0xf );
	Camera_WriteRegister(0x582a ,0x9 );
	Camera_WriteRegister(0x582b ,0x6 );
	Camera_WriteRegister(0x582c ,0x5 );
	Camera_WriteRegister(0x582d ,0x6 );
	Camera_WriteRegister(0x582e ,0xa );
	Camera_WriteRegister(0x582f ,0xe );
	Camera_WriteRegister(0x5830 ,0x28);
	Camera_WriteRegister(0x5831 ,0x1a);
	Camera_WriteRegister(0x5832 ,0x11);
	Camera_WriteRegister(0x5833 ,0xe );
	Camera_WriteRegister(0x5834 ,0xe );
	Camera_WriteRegister(0x5835 ,0xf );
	Camera_WriteRegister(0x5836 ,0x15);
	Camera_WriteRegister(0x5837 ,0x1d);
	Camera_WriteRegister(0x5838 ,0x6e);
	Camera_WriteRegister(0x5839 ,0x39);
	Camera_WriteRegister(0x583a ,0x27);
	Camera_WriteRegister(0x583b ,0x1f);
	Camera_WriteRegister(0x583c ,0x1e);
	Camera_WriteRegister(0x583d ,0x23);
	Camera_WriteRegister(0x583e ,0x2f);
	Camera_WriteRegister(0x583f ,0x41);
	Camera_WriteRegister(0x5840 ,0xe );
	Camera_WriteRegister(0x5841 ,0xc );
	Camera_WriteRegister(0x5842 ,0xd );
	Camera_WriteRegister(0x5843 ,0xc );
	Camera_WriteRegister(0x5844 ,0xc );
	Camera_WriteRegister(0x5845 ,0xc );
	Camera_WriteRegister(0x5846 ,0xc );
	Camera_WriteRegister(0x5847 ,0xc );
	Camera_WriteRegister(0x5848 ,0xd );
	Camera_WriteRegister(0x5849 ,0xe );
	Camera_WriteRegister(0x584a ,0xe );
	Camera_WriteRegister(0x584b ,0xa );
	Camera_WriteRegister(0x584c ,0xe );
	Camera_WriteRegister(0x584d ,0xe );
	Camera_WriteRegister(0x584e ,0x10);
	Camera_WriteRegister(0x584f ,0x10);
	Camera_WriteRegister(0x5850 ,0x11);
	Camera_WriteRegister(0x5851 ,0xa );
	Camera_WriteRegister(0x5852 ,0xf );
	Camera_WriteRegister(0x5853 ,0xe );
	Camera_WriteRegister(0x5854 ,0x10);
	Camera_WriteRegister(0x5855 ,0x10);
	Camera_WriteRegister(0x5856 ,0x10);
	Camera_WriteRegister(0x5857 ,0xa );
	Camera_WriteRegister(0x5858 ,0xe );
	Camera_WriteRegister(0x5859 ,0xe );
	Camera_WriteRegister(0x585a ,0xf );
	Camera_WriteRegister(0x585b ,0xf );
	Camera_WriteRegister(0x585c ,0xf );
	Camera_WriteRegister(0x585d ,0xa );
	Camera_WriteRegister(0x585e ,0x9 );
	Camera_WriteRegister(0x585f ,0xd );
	Camera_WriteRegister(0x5860 ,0xc );
	Camera_WriteRegister(0x5861 ,0xb );
	Camera_WriteRegister(0x5862 ,0xd );
	Camera_WriteRegister(0x5863 ,0x7 );
	Camera_WriteRegister(0x5864 ,0x17);
	Camera_WriteRegister(0x5865 ,0x14);
	Camera_WriteRegister(0x5866 ,0x18);
	Camera_WriteRegister(0x5867 ,0x18);
	Camera_WriteRegister(0x5868 ,0x16);
	Camera_WriteRegister(0x5869 ,0x12);
	Camera_WriteRegister(0x586a ,0x1b);
	Camera_WriteRegister(0x586b ,0x1a);
	Camera_WriteRegister(0x586c ,0x16);
	Camera_WriteRegister(0x586d ,0x16);
	Camera_WriteRegister(0x586e ,0x18);
	Camera_WriteRegister(0x586f ,0x1f);
	Camera_WriteRegister(0x5870 ,0x1c);
	Camera_WriteRegister(0x5871 ,0x16);
	Camera_WriteRegister(0x5872 ,0x10);
	Camera_WriteRegister(0x5873 ,0xf );
	Camera_WriteRegister(0x5874 ,0x13);
	Camera_WriteRegister(0x5875 ,0x1c);
	Camera_WriteRegister(0x5876 ,0x1e);
	Camera_WriteRegister(0x5877 ,0x17);
	Camera_WriteRegister(0x5878 ,0x11);
	Camera_WriteRegister(0x5879 ,0x11);
	Camera_WriteRegister(0x587a ,0x14);
	Camera_WriteRegister(0x587b ,0x1e);
	Camera_WriteRegister(0x587c ,0x1c);
	Camera_WriteRegister(0x587d ,0x1c);
	Camera_WriteRegister(0x587e ,0x1a);
	Camera_WriteRegister(0x587f ,0x1a);
	Camera_WriteRegister(0x5880 ,0x1b);
	Camera_WriteRegister(0x5881 ,0x1f);
	Camera_WriteRegister(0x5882 ,0x14);
	Camera_WriteRegister(0x5883 ,0x1a);
	Camera_WriteRegister(0x5884 ,0x1d);
	Camera_WriteRegister(0x5885 ,0x1e);
	Camera_WriteRegister(0x5886 ,0x1a);
	Camera_WriteRegister(0x5887 ,0x1a);
	//AWB
	Camera_WriteRegister(0x5180 ,0xff);
	Camera_WriteRegister(0x5181 ,0x52);
	Camera_WriteRegister(0x5182 ,0x11);
	Camera_WriteRegister(0x5183 ,0x14);
	Camera_WriteRegister(0x5184 ,0x25);
	Camera_WriteRegister(0x5185 ,0x24);
	Camera_WriteRegister(0x5186 ,0x14);
	Camera_WriteRegister(0x5187 ,0x14);
	Camera_WriteRegister(0x5188 ,0x14);
	Camera_WriteRegister(0x5189 ,0x69);
	Camera_WriteRegister(0x518a ,0x60);
	Camera_WriteRegister(0x518b ,0xa2);
	Camera_WriteRegister(0x518c ,0x9c);
	Camera_WriteRegister(0x518d ,0x36);
	Camera_WriteRegister(0x518e ,0x34);
	Camera_WriteRegister(0x518f ,0x54);
	Camera_WriteRegister(0x5190 ,0x4c);
	Camera_WriteRegister(0x5191 ,0xf8);
	Camera_WriteRegister(0x5192 ,0x04);
	Camera_WriteRegister(0x5193 ,0x70);
	Camera_WriteRegister(0x5194 ,0xf0);
	Camera_WriteRegister(0x5195 ,0xf0);
	Camera_WriteRegister(0x5196 ,0x03);
	Camera_WriteRegister(0x5197 ,0x01);
	Camera_WriteRegister(0x5198 ,0x05);
	Camera_WriteRegister(0x5199 ,0x2f);
	Camera_WriteRegister(0x519a ,0x04);
	Camera_WriteRegister(0x519b ,0x00);
	Camera_WriteRegister(0x519c ,0x06);
	Camera_WriteRegister(0x519d ,0xa0);
	Camera_WriteRegister(0x519e ,0xa0);
	//D/S
	Camera_WriteRegister(0x528a ,0x00);
	Camera_WriteRegister(0x528b ,0x01);
	Camera_WriteRegister(0x528c ,0x04);
	Camera_WriteRegister(0x528d ,0x08);
	Camera_WriteRegister(0x528e ,0x10);
	Camera_WriteRegister(0x528f ,0x20);
	Camera_WriteRegister(0x5290 ,0x30);
	Camera_WriteRegister(0x5292 ,0x00);
	Camera_WriteRegister(0x5293 ,0x00);
	Camera_WriteRegister(0x5294 ,0x00);
	Camera_WriteRegister(0x5295 ,0x01);
	Camera_WriteRegister(0x5296 ,0x00);
	Camera_WriteRegister(0x5297 ,0x04);
	Camera_WriteRegister(0x5298 ,0x00);
	Camera_WriteRegister(0x5299 ,0x08);
	Camera_WriteRegister(0x529a ,0x00);
	Camera_WriteRegister(0x529b ,0x10);
	Camera_WriteRegister(0x529c ,0x00);
	Camera_WriteRegister(0x529d ,0x20);
	Camera_WriteRegister(0x529e ,0x00);
	Camera_WriteRegister(0x529f ,0x30);
	Camera_WriteRegister(0x5282 ,0x00);
	Camera_WriteRegister(0x5300 ,0x00);
	Camera_WriteRegister(0x5301 ,0x20);
	Camera_WriteRegister(0x5302 ,0x00);
	Camera_WriteRegister(0x5303 ,0x7c);
	Camera_WriteRegister(0x530c ,0x00);
	Camera_WriteRegister(0x530d ,0x10);
	Camera_WriteRegister(0x530e ,0x20);
	Camera_WriteRegister(0x530f ,0x80);
	Camera_WriteRegister(0x5310 ,0x20);
	Camera_WriteRegister(0x5311 ,0x80);
	Camera_WriteRegister(0x5308 ,0x20);
	Camera_WriteRegister(0x5309 ,0x40);
	Camera_WriteRegister(0x5304 ,0x00);
	Camera_WriteRegister(0x5305 ,0x30);
	Camera_WriteRegister(0x5306 ,0x00);
	Camera_WriteRegister(0x5307 ,0x80);
	Camera_WriteRegister(0x5314 ,0x08);
	Camera_WriteRegister(0x5315 ,0x20);
	Camera_WriteRegister(0x5319 ,0x30);
	Camera_WriteRegister(0x5316 ,0x10);
	Camera_WriteRegister(0x5317 ,0x00);
	Camera_WriteRegister(0x5318 ,0x02);
	//CMX
	Camera_WriteRegister(0x5380 ,0x01);
	Camera_WriteRegister(0x5381 ,0x00);
	Camera_WriteRegister(0x5382 ,0x00);
	Camera_WriteRegister(0x5383 ,0x1f);
	Camera_WriteRegister(0x5384 ,0x00);
	Camera_WriteRegister(0x5385 ,0x06);
	Camera_WriteRegister(0x5386 ,0x00);
	Camera_WriteRegister(0x5387 ,0x00);
	Camera_WriteRegister(0x5388 ,0x00);
	Camera_WriteRegister(0x5389 ,0xE1);
	Camera_WriteRegister(0x538A ,0x00);
	Camera_WriteRegister(0x538B ,0x2B);
	Camera_WriteRegister(0x538C ,0x00);
	Camera_WriteRegister(0x538D ,0x00);
	Camera_WriteRegister(0x538E ,0x00);
	Camera_WriteRegister(0x538F ,0x10);
	Camera_WriteRegister(0x5390 ,0x00);
	Camera_WriteRegister(0x5391 ,0xB3);
	Camera_WriteRegister(0x5392 ,0x00);
	Camera_WriteRegister(0x5393 ,0xA6);
	Camera_WriteRegister(0x5394 ,0x08);
	//GAMMA
	Camera_WriteRegister(0x5480 ,0x0c);
	Camera_WriteRegister(0x5481 ,0x18);
	Camera_WriteRegister(0x5482 ,0x2f);
	Camera_WriteRegister(0x5483 ,0x55);
	Camera_WriteRegister(0x5484 ,0x64);
	Camera_WriteRegister(0x5485 ,0x71);
	Camera_WriteRegister(0x5486 ,0x7d);
	Camera_WriteRegister(0x5487 ,0x87);
	Camera_WriteRegister(0x5488 ,0x91);
	Camera_WriteRegister(0x5489 ,0x9a);
	Camera_WriteRegister(0x548A ,0xaa);
	Camera_WriteRegister(0x548B ,0xb8);
	Camera_WriteRegister(0x548C ,0xcd);
	Camera_WriteRegister(0x548D ,0xdd);
	Camera_WriteRegister(0x548E ,0xea);
	Camera_WriteRegister(0x548F ,0x1d);
	Camera_WriteRegister(0x5490 ,0x05);
	Camera_WriteRegister(0x5491 ,0x00);
	Camera_WriteRegister(0x5492 ,0x04);
	Camera_WriteRegister(0x5493 ,0x20);
	Camera_WriteRegister(0x5494 ,0x03);
	Camera_WriteRegister(0x5495 ,0x60);
	Camera_WriteRegister(0x5496 ,0x02);
	Camera_WriteRegister(0x5497 ,0xB8);
	Camera_WriteRegister(0x5498 ,0x02);
	Camera_WriteRegister(0x5499 ,0x86);
	Camera_WriteRegister(0x549A ,0x02);
	Camera_WriteRegister(0x549B ,0x5B);
	Camera_WriteRegister(0x549C ,0x02);
	Camera_WriteRegister(0x549D ,0x3B);
	Camera_WriteRegister(0x549E ,0x02);
	Camera_WriteRegister(0x549F ,0x1C);
	Camera_WriteRegister(0x54A0 ,0x02);
	Camera_WriteRegister(0x54A1 ,0x04);
	Camera_WriteRegister(0x54A2 ,0x01);
	Camera_WriteRegister(0x54A3 ,0xED);
	Camera_WriteRegister(0x54A4 ,0x01);
	Camera_WriteRegister(0x54A5 ,0xC5);
	Camera_WriteRegister(0x54A6 ,0x01);
	Camera_WriteRegister(0x54A7 ,0xA5);
	Camera_WriteRegister(0x54A8 ,0x01);
	Camera_WriteRegister(0x54A9 ,0x6C);
	Camera_WriteRegister(0x54AA ,0x01);
	Camera_WriteRegister(0x54AB ,0x41);
	Camera_WriteRegister(0x54AC ,0x01);
	Camera_WriteRegister(0x54AD ,0x20);
	Camera_WriteRegister(0x54AE ,0x00);
	Camera_WriteRegister(0x54AF ,0x16);
	Camera_WriteRegister(0x54B0 ,0x01);
	Camera_WriteRegister(0x54B1 ,0x20);
	Camera_WriteRegister(0x54B2 ,0x00);
	Camera_WriteRegister(0x54B3 ,0x10);
	Camera_WriteRegister(0x54B4 ,0x00);
	Camera_WriteRegister(0x54B5 ,0xf0);
	Camera_WriteRegister(0x54B6 ,0x00);
	Camera_WriteRegister(0x54B7 ,0xDF);
	Camera_WriteRegister(0x5402 ,0x3f);
	Camera_WriteRegister(0x5403 ,0x00);
	//UV ADJUST
	Camera_WriteRegister(0x5500 ,0x10);
	Camera_WriteRegister(0x5502 ,0x00);
	Camera_WriteRegister(0x5503 ,0x06);
	Camera_WriteRegister(0x5504 ,0x00);
	Camera_WriteRegister(0x5505 ,0x7f);
	//AE
	Camera_WriteRegister(0x5025 ,0x80);
	Camera_WriteRegister(0x3a0f ,0x30);
	Camera_WriteRegister(0x3a10 ,0x28);
	Camera_WriteRegister(0x3a1b ,0x30);
	Camera_WriteRegister(0x3a1e ,0x28);
	Camera_WriteRegister(0x3a11 ,0x61);
	Camera_WriteRegister(0x3a1f ,0x10);
	Camera_WriteRegister(0x5688 ,0xfd);
	Camera_WriteRegister(0x5689 ,0xdf);
	Camera_WriteRegister(0x568a ,0xfe);
	Camera_WriteRegister(0x568b ,0xef);
	Camera_WriteRegister(0x568c ,0xfe);
	Camera_WriteRegister(0x568d ,0xef);
	Camera_WriteRegister(0x568e ,0xaa);		
	
	
}

void SVGA_PreView( void )
{
	Camera_WriteRegister(0x3800 ,0x1 );
	Camera_WriteRegister(0x3801 ,0x50);
	Camera_WriteRegister(0x3802 ,0x0 );
	Camera_WriteRegister(0x3803 ,0x8 );
	Camera_WriteRegister(0x3804 ,0x5 );
	Camera_WriteRegister(0x3805 ,0x0 );
	Camera_WriteRegister(0x3806 ,0x3 );
	Camera_WriteRegister(0x3807 ,0xc0);
	Camera_WriteRegister(0x3808 ,0x3 );
	Camera_WriteRegister(0x3809 ,0x20);
	Camera_WriteRegister(0x380a ,0x2 );
	Camera_WriteRegister(0x380b ,0x58);
	Camera_WriteRegister(0x380c ,0xc );
	Camera_WriteRegister(0x380d ,0x80);
	Camera_WriteRegister(0x380e ,0x3 );
	Camera_WriteRegister(0x380f ,0xe8);
	Camera_WriteRegister(0x5001 ,0x7f);
	Camera_WriteRegister(0x5680 ,0x0 );
	Camera_WriteRegister(0x5681 ,0x0 );
	Camera_WriteRegister(0x5682 ,0x5 );
	Camera_WriteRegister(0x5683 ,0x0 );
	Camera_WriteRegister(0x5684 ,0x0 );
	Camera_WriteRegister(0x5685 ,0x0 );
	Camera_WriteRegister(0x5686 ,0x3 );
	Camera_WriteRegister(0x5687 ,0xc0);
	Camera_WriteRegister(0x5687 ,0xc0);
	Camera_WriteRegister(0x3815 ,0x02);	
	Camera_WriteRegister(0x4300 ,0x32);//YUV 422, UYVY 설정
	Camera_WriteRegister(0x4730,0x01);//CCIR656 MODE ENABLE
	
}
void QCIG_PreView( void )
{
	Camera_WriteRegister(0x3800 ,0x1 );
	Camera_WriteRegister(0x3801 ,0x50);
	Camera_WriteRegister(0x3802 ,0x0 );
	Camera_WriteRegister(0x3803 ,0x8 );
	Camera_WriteRegister(0x3804 ,0x4 );
	Camera_WriteRegister(0x3805 ,0x96);
	Camera_WriteRegister(0x3806 ,0x3 );
	Camera_WriteRegister(0x3807 ,0xc0);
	Camera_WriteRegister(0x3808 ,0x0 );
	Camera_WriteRegister(0x3809 ,0xb0);
	Camera_WriteRegister(0x380a ,0x0 );
	Camera_WriteRegister(0x380b ,0x90);
	Camera_WriteRegister(0x380c ,0xc );
	Camera_WriteRegister(0x380d ,0x80);
	Camera_WriteRegister(0x380e ,0x3 );
	Camera_WriteRegister(0x380f ,0xe8);
	Camera_WriteRegister(0x5001 ,0x7f);
	Camera_WriteRegister(0x5680 ,0x0 );
	Camera_WriteRegister(0x5681 ,0x0 );
	Camera_WriteRegister(0x5682 ,0x4 );
	Camera_WriteRegister(0x5683 ,0x96);
	Camera_WriteRegister(0x5684 ,0x0 );
	Camera_WriteRegister(0x5685 ,0x0 );		
	Camera_WriteRegister(0x4300 ,0x32);//YUV 422, UYVY 설정
	Camera_WriteRegister(0x4730,0x01);//CCIR656 MODE ENABLE
	
}
void  CIF_PreView( void )
{
	Camera_WriteRegister(0x3800 ,0x1 );
	Camera_WriteRegister(0x3801 ,0x50);
	Camera_WriteRegister(0x3802 ,0x0 );
	Camera_WriteRegister(0x3803 ,0x8 );
	Camera_WriteRegister(0x3804 ,0x4 );
	Camera_WriteRegister(0x3805 ,0x96);
	Camera_WriteRegister(0x3806 ,0x3 );
	Camera_WriteRegister(0x3807 ,0xc0);
	Camera_WriteRegister(0x3808 ,0x1 );
	Camera_WriteRegister(0x3809 ,0x60);
	Camera_WriteRegister(0x380a ,0x1 );
	Camera_WriteRegister(0x380b ,0x20);
	Camera_WriteRegister(0x380c ,0xc );
	Camera_WriteRegister(0x380d ,0x80);
	Camera_WriteRegister(0x380e ,0x3 );
	Camera_WriteRegister(0x380f ,0xe8);

	
	Camera_WriteRegister(0x5001 ,0x7f);
	Camera_WriteRegister(0x5680 ,0x0 );
	Camera_WriteRegister(0x5681 ,0x0 );
	Camera_WriteRegister(0x5682 ,0x4 );
	Camera_WriteRegister(0x5683 ,0x96);
	Camera_WriteRegister(0x5684 ,0x0 );
	Camera_WriteRegister(0x5685 ,0x0 );
	Camera_WriteRegister(0x5686 ,0x3 );
	Camera_WriteRegister(0x5687 ,0xc0);
	Camera_WriteRegister(0x4300 ,0x32);//YUV 422, UYVY 설정
	Camera_WriteRegister(0x4730,0x01);//CCIR656 MODE ENABLE
	
}

void QSXGA_to_vga640_480( void )
{
	Camera_WriteRegister(0x3800 ,0x1 );
	Camera_WriteRegister(0x3801 ,0x8A);
	Camera_WriteRegister(0x3802 ,0x0 );
	Camera_WriteRegister(0x3803 ,0xA );
	Camera_WriteRegister(0x3804 ,0xA );
	Camera_WriteRegister(0x3805 ,0x20);
	Camera_WriteRegister(0x3806 ,0x7 );
	Camera_WriteRegister(0x3807 ,0x98);
	Camera_WriteRegister(0x3808 ,0x2 );
	Camera_WriteRegister(0x3809 ,0x80);
	Camera_WriteRegister(0x380a ,0x1 );
	Camera_WriteRegister(0x380b ,0xe0);
	Camera_WriteRegister(0x380c ,0xc );
	Camera_WriteRegister(0x380d ,0x80);
	Camera_WriteRegister(0x380e ,0x7 );
	Camera_WriteRegister(0x380f ,0xd0);
	Camera_WriteRegister(0x5001 ,0x7f);
	Camera_WriteRegister(0x5680 ,0x0 );
	Camera_WriteRegister(0x5681 ,0x0 );
	Camera_WriteRegister(0x5682 ,0xA );
	Camera_WriteRegister(0x5683 ,0x20);
	Camera_WriteRegister(0x5684 ,0x0 );
	Camera_WriteRegister(0x5685 ,0x0 );
	Camera_WriteRegister(0x5686 ,0x7 );
	Camera_WriteRegister(0x5687 ,0x98);
	Camera_WriteRegister(0x4300 ,0x32);//YUV 422, UYVY 설정
	Camera_WriteRegister(0x4730,0x01);//CCIR656 MODE ENABLE
	
}

void QSXGA_to_sxga1280_960( void )
{
	Camera_WriteRegister(0x3800 ,0x1 );
	Camera_WriteRegister(0x3801 ,0x8A);
	Camera_WriteRegister(0x3802 ,0x0 );
	Camera_WriteRegister(0x3803 ,0xA );
	Camera_WriteRegister(0x3804 ,0xA );
	Camera_WriteRegister(0x3805 ,0x20);
	Camera_WriteRegister(0x3806 ,0x7 );
	Camera_WriteRegister(0x3807 ,0x98);
	Camera_WriteRegister(0x3808 ,0x5 );
	Camera_WriteRegister(0x3809 ,0x0 );
	Camera_WriteRegister(0x380a ,0x3 );
	Camera_WriteRegister(0x380b ,0xc0);
	Camera_WriteRegister(0x380c ,0xc );
	Camera_WriteRegister(0x380d ,0x80);
	Camera_WriteRegister(0x380e ,0x7 );
	Camera_WriteRegister(0x380f ,0xd0);
	Camera_WriteRegister(0x5001 ,0x7f);
	Camera_WriteRegister(0x5680 ,0x0 );
	Camera_WriteRegister(0x5681 ,0x0 );
	Camera_WriteRegister(0x5682 ,0xA );
	Camera_WriteRegister(0x5683 ,0x20);
	Camera_WriteRegister(0x5684 ,0x0 );
	Camera_WriteRegister(0x5685 ,0x0 );
	Camera_WriteRegister(0x5686 ,0x7 );
	Camera_WriteRegister(0x5687 ,0x98);
	Camera_WriteRegister(0x4300 ,0x32);//YUV 422, UYVY 설정
	Camera_WriteRegister(0x4730,0x01);//CCIR656 MODE ENABLE
}

void QSXGA_to_QVGA320_240( void )
{
	Camera_WriteRegister(0x3800 ,0x1 );
	Camera_WriteRegister(0x3801 ,0x8A);
	Camera_WriteRegister(0x3802 ,0x0 );
	Camera_WriteRegister(0x3803 ,0xA );

	Camera_WriteRegister(0x3804 ,0xA );
	Camera_WriteRegister(0x3805 ,0x20);
	Camera_WriteRegister(0x3806 ,0x7 );
	Camera_WriteRegister(0x3807 ,0x98);

	Camera_WriteRegister(0x3808 ,0x1 );
	Camera_WriteRegister(0x3809 ,0x40);
	Camera_WriteRegister(0x380a ,0x0 );
	Camera_WriteRegister(0x380b ,0xf0);

	Camera_WriteRegister(0x380c ,0xc );
	Camera_WriteRegister(0x380d ,0x80);
	Camera_WriteRegister(0x380e ,0x7 );
	Camera_WriteRegister(0x380f ,0xd0);

	Camera_WriteRegister(0x5001 ,0x7f);
	Camera_WriteRegister(0x5680 ,0x0 );
	Camera_WriteRegister(0x5681 ,0x0 );
	Camera_WriteRegister(0x5682 ,0xA );
	Camera_WriteRegister(0x5683 ,0x20);
	Camera_WriteRegister(0x5684 ,0x0 );
	Camera_WriteRegister(0x5685 ,0x0 );
	Camera_WriteRegister(0x5686 ,0x7 );
	Camera_WriteRegister(0x5687 ,0x98);

	Camera_WriteRegister(0x4300 ,0x32);//YUV 422, UYVY 설정
	Camera_WriteRegister(0x4730,0x01);//CCIR656 MODE ENABLE
}

void QSXGA_to_qxga2048_1536( void )
{
	Camera_WriteRegister(0x3800 ,0x1 );
	Camera_WriteRegister(0x3801 ,0x8A);
	Camera_WriteRegister(0x3802 ,0x0 );
	Camera_WriteRegister(0x3803 ,0xA );
	Camera_WriteRegister(0x3804 ,0xA );
	Camera_WriteRegister(0x3805 ,0x20);
	Camera_WriteRegister(0x3806 ,0x7 );
	Camera_WriteRegister(0x3807 ,0x98);
	Camera_WriteRegister(0x3808 ,0x8 );
	Camera_WriteRegister(0x3809 ,0x0 );
	Camera_WriteRegister(0x380a ,0x6 );
	Camera_WriteRegister(0x380b ,0x0 );
	Camera_WriteRegister(0x380c ,0xc );
	Camera_WriteRegister(0x380d ,0x80);
	Camera_WriteRegister(0x380e ,0x7 );
	Camera_WriteRegister(0x380f ,0xd0);
	Camera_WriteRegister(0x5001 ,0x7f);
	Camera_WriteRegister(0x5680 ,0x0 );
	Camera_WriteRegister(0x5681 ,0x0 );
	Camera_WriteRegister(0x5682 ,0xA );
	Camera_WriteRegister(0x5683 ,0x20);
	Camera_WriteRegister(0x5684 ,0x0 );
	Camera_WriteRegister(0x5685 ,0x0 );
	Camera_WriteRegister(0x5686 ,0x7 );
	Camera_WriteRegister(0x5687 ,0x98);
	Camera_WriteRegister(0x4300 ,0x32);//YUV 422, UYVY 설정
	Camera_WriteRegister(0x4730,0x01);//CCIR656 MODE ENABLE	
}

void QSXGA_to_uxga1600_1200( void )
{
	Camera_WriteRegister(0x3800 ,0x1 );
	Camera_WriteRegister(0x3801 ,0x8A);
	Camera_WriteRegister(0x3802 ,0x0 );
	Camera_WriteRegister(0x3803 ,0xA );
	Camera_WriteRegister(0x3804 ,0xA );
	Camera_WriteRegister(0x3805 ,0x20);
	Camera_WriteRegister(0x3806 ,0x7 );
	Camera_WriteRegister(0x3807 ,0x98);
	Camera_WriteRegister(0x3808 ,0x6 );
	Camera_WriteRegister(0x3809 ,0x40);
	Camera_WriteRegister(0x380a ,0x4 );
	Camera_WriteRegister(0x380b ,0xb0);
	Camera_WriteRegister(0x380c ,0xc );
	Camera_WriteRegister(0x380d ,0x80);
	Camera_WriteRegister(0x380e ,0x7 );
	Camera_WriteRegister(0x380f ,0xd0);
	Camera_WriteRegister(0x5001 ,0x7f);
	Camera_WriteRegister(0x5680 ,0x0 );
	Camera_WriteRegister(0x5681 ,0x0 );
	Camera_WriteRegister(0x5682 ,0xA );
	Camera_WriteRegister(0x5683 ,0x20);
	Camera_WriteRegister(0x5684 ,0x0 );
	Camera_WriteRegister(0x5685 ,0x0 );
	Camera_WriteRegister(0x5686 ,0x7 );
	Camera_WriteRegister(0x5687 ,0x98);
	Camera_WriteRegister(0x4300 ,0x32);//YUV 422, UYVY 설정
	Camera_WriteRegister(0x4730,0x01);//CCIR656 MODE ENABLE	

}

void QSXGA_to_xga1024_768( void )
{
	Camera_WriteRegister(0x3800 ,0x01 );
	Camera_WriteRegister(0x3801 ,0x8a);
	Camera_WriteRegister(0x3802 ,0x00 );
	Camera_WriteRegister(0x3803 ,0x0a );
	Camera_WriteRegister(0x3804 ,0xA );
	Camera_WriteRegister(0x3805 ,0x20);
	Camera_WriteRegister(0x3806 ,0x7 );
	Camera_WriteRegister(0x3807 ,0x98);
	Camera_WriteRegister(0x3808 ,0x4 );
	Camera_WriteRegister(0x3809 ,0x0 );
	Camera_WriteRegister(0x380a ,0x3 );
	Camera_WriteRegister(0x380b ,0x0 );
	Camera_WriteRegister(0x380c ,0xc );
	Camera_WriteRegister(0x380d ,0x80);
	Camera_WriteRegister(0x380e ,0x7 );
	Camera_WriteRegister(0x380f ,0xd0);
	/*
	Camera_WriteRegister(0x5001 ,0x7f);
	Camera_WriteRegister(0x5680 ,0x0 );
	Camera_WriteRegister(0x5681 ,0x0 );
	Camera_WriteRegister(0x5682 ,0xA );
	Camera_WriteRegister(0x5683 ,0x20);
	Camera_WriteRegister(0x5684 ,0x0 );
	Camera_WriteRegister(0x5685 ,0x0 );
	Camera_WriteRegister(0x5686 ,0x7 );
	Camera_WriteRegister(0x5687 ,0x98);
	Camera_WriteRegister(0x4300 ,0x32);//YUV 422, UYVY 설정
	Camera_WriteRegister(0x4730,0x01);//CCIR656 MODE ENABLE	
	*/ 
}

void QSXGA_Capture( void )
{
	Camera_WriteRegister(0x3503 ,0x07);
	Camera_WriteRegister(0x3000 ,0x00);
	Camera_WriteRegister(0x3001 ,0x00);
	Camera_WriteRegister(0x3002 ,0x00);
	Camera_WriteRegister(0x3003 ,0x00);
	Camera_WriteRegister(0x3005 ,0xff);
	Camera_WriteRegister(0x3006 ,0xff);
	Camera_WriteRegister(0x3007 ,0x3f);
	Camera_WriteRegister(0x350c ,0x07);
	Camera_WriteRegister(0x350d ,0xd0);
	Camera_WriteRegister(0x3602 ,0xe4);
	Camera_WriteRegister(0x3612 ,0xac);
	Camera_WriteRegister(0x3613 ,0x44);
	Camera_WriteRegister(0x3621 ,0x27);
	Camera_WriteRegister(0x3622 ,0x08);
	Camera_WriteRegister(0x3623 ,0x22);
	Camera_WriteRegister(0x3604 ,0x60);
	Camera_WriteRegister(0x3705 ,0xda);
	Camera_WriteRegister(0x370a ,0x80);
	Camera_WriteRegister(0x3801 ,0x8a);
	Camera_WriteRegister(0x3803 ,0x0a);
	Camera_WriteRegister(0x3804 ,0x0a);
	Camera_WriteRegister(0x3805 ,0x20);
	Camera_WriteRegister(0x3806 ,0x07);
	Camera_WriteRegister(0x3807 ,0x98);
	Camera_WriteRegister(0x3808 ,0x0a);
	Camera_WriteRegister(0x3809 ,0x20);
	Camera_WriteRegister(0x380a ,0x07);
	Camera_WriteRegister(0x380b ,0x98);
	Camera_WriteRegister(0x380c ,0x0c);
	Camera_WriteRegister(0x380d ,0x80);
	Camera_WriteRegister(0x380e ,0x07);
	Camera_WriteRegister(0x380f ,0xd0);
	Camera_WriteRegister(0x3810 ,0xc2);
	Camera_WriteRegister(0x3815 ,0x44);
	Camera_WriteRegister(0x3818 ,0xc8);
	Camera_WriteRegister(0x3824 ,0x01);
	Camera_WriteRegister(0x3827 ,0x0a);
	Camera_WriteRegister(0x3a00 ,0x78);
	Camera_WriteRegister(0x3a0d ,0x10);
	Camera_WriteRegister(0x3a0e ,0x0d);
	Camera_WriteRegister(0x3a10 ,0x32);
	Camera_WriteRegister(0x3a1b ,0x3c);
	Camera_WriteRegister(0x3a1e ,0x32);
	Camera_WriteRegister(0x3a11 ,0x80);
	Camera_WriteRegister(0x3a1f ,0x20);
	Camera_WriteRegister(0x3a00 ,0x78);
	Camera_WriteRegister(0x460b ,0x35);
	Camera_WriteRegister(0x471d ,0x00);
	Camera_WriteRegister(0x4713 ,0x03);
	Camera_WriteRegister(0x471c ,0x50);
	Camera_WriteRegister(0x5682 ,0x0a);
	Camera_WriteRegister(0x5683 ,0x20);
	Camera_WriteRegister(0x5686 ,0x07);
	Camera_WriteRegister(0x5687 ,0x98);
	Camera_WriteRegister(0x5001 ,0x4f);
	Camera_WriteRegister(0x589b ,0x00);
	Camera_WriteRegister(0x589a ,0xc0);
	Camera_WriteRegister(0x4407 ,0x04);
	Camera_WriteRegister(0x589b ,0x00);
	Camera_WriteRegister(0x589a ,0xc0);
	Camera_WriteRegister(0x3002 ,0x0c);
	Camera_WriteRegister(0x3002 ,0x00);
	Camera_WriteRegister(0x3503 ,0x00);			
}
void High_Resolution_Video_720P( void )
{
Camera_WriteRegister(0x3103 ,0x93);
Camera_WriteRegister(0x3008 ,0x82);
Camera_WriteRegister(0x3017 ,0x7f);
Camera_WriteRegister(0x3018 ,0xfc);
Camera_WriteRegister(0x3810 ,0xc2);
Camera_WriteRegister(0x3615 ,0xf0);
Camera_WriteRegister(0x3000 ,0x00);
Camera_WriteRegister(0x3001 ,0x00);
Camera_WriteRegister(0x3002 ,0x00);
Camera_WriteRegister(0x3003 ,0x00);
Camera_WriteRegister(0x3004 ,0xff);
Camera_WriteRegister(0x3030 ,0x2b);
Camera_WriteRegister(0x3011 ,0x08);
Camera_WriteRegister(0x3010 ,0x10);
Camera_WriteRegister(0x3604 ,0x60);
Camera_WriteRegister(0x3622 ,0x60);
Camera_WriteRegister(0x3621 ,0x09);
Camera_WriteRegister(0x3709 ,0x00);
Camera_WriteRegister(0x4000 ,0x21);
Camera_WriteRegister(0x401d ,0x22);
Camera_WriteRegister(0x3600 ,0x54);
Camera_WriteRegister(0x3605 ,0x04);
Camera_WriteRegister(0x3606 ,0x3f);
Camera_WriteRegister(0x3c01 ,0x80);
Camera_WriteRegister(0x300d ,0x22);
Camera_WriteRegister(0x3623 ,0x22);
Camera_WriteRegister(0x5000 ,0x4f);
Camera_WriteRegister(0x5020 ,0x04);
Camera_WriteRegister(0x5181 ,0x79);
Camera_WriteRegister(0x5182 ,0x00);
Camera_WriteRegister(0x5185 ,0x22);
Camera_WriteRegister(0x5197 ,0x01);
Camera_WriteRegister(0x5500 ,0x0a);
Camera_WriteRegister(0x5504 ,0x00);
Camera_WriteRegister(0x5505 ,0x7f);
Camera_WriteRegister(0x5080 ,0x08);
Camera_WriteRegister(0x300e ,0x18);
Camera_WriteRegister(0x4610 ,0x00);
Camera_WriteRegister(0x471d ,0x05);
Camera_WriteRegister(0x4708 ,0x06);
Camera_WriteRegister(0x370c ,0xa0);
Camera_WriteRegister(0x3808 ,0x0a);
Camera_WriteRegister(0x3809 ,0x20);
Camera_WriteRegister(0x380a ,0x07);
Camera_WriteRegister(0x380b ,0x98);
Camera_WriteRegister(0x380c ,0x0c);
Camera_WriteRegister(0x380d ,0x80);
Camera_WriteRegister(0x380e ,0x07);
Camera_WriteRegister(0x380f ,0xd0);
Camera_WriteRegister(0x5687 ,0x94);
Camera_WriteRegister(0x501f ,0x00);
Camera_WriteRegister(0x5000 ,0x4f);
Camera_WriteRegister(0x5001 ,0xcf);
Camera_WriteRegister(0x4300 ,0x30);
Camera_WriteRegister(0x4300 ,0x30);
Camera_WriteRegister(0x460b ,0x35);
Camera_WriteRegister(0x471d ,0x00);
Camera_WriteRegister(0x3002 ,0x0c);
Camera_WriteRegister(0x3002 ,0x00);
Camera_WriteRegister(0x4713 ,0x03);
Camera_WriteRegister(0x471c ,0x50);
Camera_WriteRegister(0x4721 ,0x02);
Camera_WriteRegister(0x4402 ,0x90);
Camera_WriteRegister(0x460c ,0x22);
Camera_WriteRegister(0x3815 ,0x44);
Camera_WriteRegister(0x3503 ,0x07);
Camera_WriteRegister(0x3501 ,0x73);
Camera_WriteRegister(0x3502 ,0x80);
Camera_WriteRegister(0x350b ,0x00);
Camera_WriteRegister(0x3818 ,0xc8);
Camera_WriteRegister(0x3801 ,0x88);
Camera_WriteRegister(0x3824 ,0x11);
Camera_WriteRegister(0x3a00 ,0x78);
Camera_WriteRegister(0x3a1a ,0x04);
Camera_WriteRegister(0x3a13 ,0x30);
Camera_WriteRegister(0x3a18 ,0x00);
Camera_WriteRegister(0x3a19 ,0x7c);
Camera_WriteRegister(0x3a08 ,0x12);
Camera_WriteRegister(0x3a09 ,0xc0);
Camera_WriteRegister(0x3a0a ,0x0f);
Camera_WriteRegister(0x3a0b ,0xa0);
Camera_WriteRegister(0x350c ,0x07);
Camera_WriteRegister(0x350d ,0xd0);
Camera_WriteRegister(0x3a0d ,0x08);
Camera_WriteRegister(0x3a0e ,0x06);
Camera_WriteRegister(0x3500 ,0x00);
Camera_WriteRegister(0x3501 ,0x00);
Camera_WriteRegister(0x3502 ,0x00);
Camera_WriteRegister(0x350a ,0x00);
Camera_WriteRegister(0x350b ,0x00);
Camera_WriteRegister(0x3503 ,0x00);
Camera_WriteRegister(0x3030 ,0x2b);
Camera_WriteRegister(0x3a02 ,0x00);
Camera_WriteRegister(0x3a03 ,0x7d);
Camera_WriteRegister(0x3a04 ,0x00);
Camera_WriteRegister(0x3a14 ,0x00);
Camera_WriteRegister(0x3a15 ,0x7d);
Camera_WriteRegister(0x3a16 ,0x00);
Camera_WriteRegister(0x3a00 ,0x78);
Camera_WriteRegister(0x3a08 ,0x09);
Camera_WriteRegister(0x3a09 ,0x60);
Camera_WriteRegister(0x3a0a ,0x07);
Camera_WriteRegister(0x3a0b ,0xd0);
Camera_WriteRegister(0x3a0d ,0x10);
Camera_WriteRegister(0x3a0e ,0x0d);
Camera_WriteRegister(0x4407 ,0x04);
Camera_WriteRegister(0x5193 ,0x70);
Camera_WriteRegister(0x589b ,0x00);
Camera_WriteRegister(0x589a ,0xc0);
Camera_WriteRegister(0x401e ,0x20);
Camera_WriteRegister(0x4001 ,0x42);
Camera_WriteRegister(0x401c ,0x06);
Camera_WriteRegister(0x3825 ,0xac);
Camera_WriteRegister(0x3827 ,0x0c);
Camera_WriteRegister(0x5402 ,0x3f);
Camera_WriteRegister(0x5403 ,0x00);
Camera_WriteRegister(0x3406 ,0x00);
Camera_WriteRegister(0x5025 ,0x80);
Camera_WriteRegister(0x5583 ,0x40);
Camera_WriteRegister(0x5584 ,0x40);
Camera_WriteRegister(0x5580 ,0x02);
Camera_WriteRegister(0x5000 ,0xcf);
Camera_WriteRegister(0x3710 ,0x10);
Camera_WriteRegister(0x3632 ,0x51);
Camera_WriteRegister(0x3702 ,0x10);
Camera_WriteRegister(0x3703 ,0xb2);
Camera_WriteRegister(0x3704 ,0x18);
Camera_WriteRegister(0x370b ,0x40);
Camera_WriteRegister(0x370d ,0x03);
Camera_WriteRegister(0x3631 ,0x01);
Camera_WriteRegister(0x3632 ,0x52);
Camera_WriteRegister(0x3606 ,0x24);
Camera_WriteRegister(0x3620 ,0x96);
Camera_WriteRegister(0x5785 ,0x07);
Camera_WriteRegister(0x3a13 ,0x30);
Camera_WriteRegister(0x3600 ,0x52);
Camera_WriteRegister(0x3604 ,0x48);
Camera_WriteRegister(0x3606 ,0x1b);
Camera_WriteRegister(0x370d ,0x0b);
Camera_WriteRegister(0x370f ,0xc0);
Camera_WriteRegister(0x3709 ,0x01);
Camera_WriteRegister(0x3823 ,0x00);
Camera_WriteRegister(0x5007 ,0x00);
Camera_WriteRegister(0x5009 ,0x00);
Camera_WriteRegister(0x5011 ,0x00);
Camera_WriteRegister(0x5013 ,0x00);
Camera_WriteRegister(0x519e ,0x00);
Camera_WriteRegister(0x5086 ,0x00);
Camera_WriteRegister(0x5087 ,0x00);
Camera_WriteRegister(0x5088 ,0x00);
Camera_WriteRegister(0x5089 ,0x00);
Camera_WriteRegister(0x302b ,0x00);
Camera_WriteRegister(0x3503 ,0x07);
Camera_WriteRegister(0x3011 ,0x08);
Camera_WriteRegister(0x350c ,0x02);
Camera_WriteRegister(0x350d ,0xe4);
Camera_WriteRegister(0x3621 ,0xc9);
Camera_WriteRegister(0x370a ,0x81);
Camera_WriteRegister(0x3803 ,0x08);
Camera_WriteRegister(0x3804 ,0x05);
Camera_WriteRegister(0x3805 ,0x00);
Camera_WriteRegister(0x3806 ,0x02);
Camera_WriteRegister(0x3807 ,0xd0);
Camera_WriteRegister(0x3808 ,0x05);
Camera_WriteRegister(0x3809 ,0x00);
Camera_WriteRegister(0x380a ,0x02);
Camera_WriteRegister(0x380b ,0xd0);
Camera_WriteRegister(0x380c ,0x08);
Camera_WriteRegister(0x380d ,0x72);
Camera_WriteRegister(0x380e ,0x02);
Camera_WriteRegister(0x380f ,0xe4);
Camera_WriteRegister(0x3810 ,0xc0);
Camera_WriteRegister(0x3818 ,0xc9);
Camera_WriteRegister(0x381c ,0x10);
Camera_WriteRegister(0x381d ,0xa0);
Camera_WriteRegister(0x381e ,0x05);
Camera_WriteRegister(0x381f ,0xb0);
Camera_WriteRegister(0x3820 ,0x00);
Camera_WriteRegister(0x3821 ,0x00);
Camera_WriteRegister(0x3824 ,0x11);
Camera_WriteRegister(0x3a08 ,0x1b);
Camera_WriteRegister(0x3a09 ,0xc0);
Camera_WriteRegister(0x3a0a ,0x17);
Camera_WriteRegister(0x3a0b ,0x20);
Camera_WriteRegister(0x3a0d ,0x02);
Camera_WriteRegister(0x3a0e ,0x01);
Camera_WriteRegister(0x401c ,0x04);
Camera_WriteRegister(0x5682 ,0x05);
Camera_WriteRegister(0x5683 ,0x00);
Camera_WriteRegister(0x5686 ,0x02);
Camera_WriteRegister(0x5687 ,0xcc);
Camera_WriteRegister(0x5001 ,0x7f);
Camera_WriteRegister(0x589b ,0x06);
Camera_WriteRegister(0x589a ,0xc5);
Camera_WriteRegister(0x3503 ,0x00);
Camera_WriteRegister(0x3010 ,0x10);
Camera_WriteRegister(0x5001 ,0xFF);
Camera_WriteRegister(0x5583 ,0x50);
Camera_WriteRegister(0x5584 ,0x50);
Camera_WriteRegister(0x5580 ,0x02);
Camera_WriteRegister(0x3c01 ,0x80);
Camera_WriteRegister(0x3c00 ,0x04);
//LENS
Camera_WriteRegister(0x5800 ,0x48);
Camera_WriteRegister(0x5801 ,0x31);
Camera_WriteRegister(0x5802 ,0x21);
Camera_WriteRegister(0x5803 ,0x1b);
Camera_WriteRegister(0x5804 ,0x1a);
Camera_WriteRegister(0x5805 ,0x1e);
Camera_WriteRegister(0x5806 ,0x29);
Camera_WriteRegister(0x5807 ,0x38);
Camera_WriteRegister(0x5808 ,0x26);
Camera_WriteRegister(0x5809 ,0x17);
Camera_WriteRegister(0x580a ,0x11);
Camera_WriteRegister(0x580b ,0xe );
Camera_WriteRegister(0x580c ,0xd );
Camera_WriteRegister(0x580d ,0xe );
Camera_WriteRegister(0x580e ,0x13);
Camera_WriteRegister(0x580f ,0x1a);
Camera_WriteRegister(0x5810 ,0x15);
Camera_WriteRegister(0x5811 ,0xd );
Camera_WriteRegister(0x5812 ,0x8 );
Camera_WriteRegister(0x5813 ,0x5 );
Camera_WriteRegister(0x5814 ,0x4 );
Camera_WriteRegister(0x5815 ,0x5 );
Camera_WriteRegister(0x5816 ,0x9 );
Camera_WriteRegister(0x5817 ,0xd );
Camera_WriteRegister(0x5818 ,0x11);
Camera_WriteRegister(0x5819 ,0xa );
Camera_WriteRegister(0x581a ,0x4 );
Camera_WriteRegister(0x581b ,0x0 );
Camera_WriteRegister(0x581c ,0x0 );
Camera_WriteRegister(0x581d ,0x1 );
Camera_WriteRegister(0x581e ,0x6 );
Camera_WriteRegister(0x581f ,0x9 );
Camera_WriteRegister(0x5820 ,0x12);
Camera_WriteRegister(0x5821 ,0xb );
Camera_WriteRegister(0x5822 ,0x4 );
Camera_WriteRegister(0x5823 ,0x0 );
Camera_WriteRegister(0x5824 ,0x0 );
Camera_WriteRegister(0x5825 ,0x1 );
Camera_WriteRegister(0x5826 ,0x6 );
Camera_WriteRegister(0x5827 ,0xa );
Camera_WriteRegister(0x5828 ,0x17);
Camera_WriteRegister(0x5829 ,0xf );
Camera_WriteRegister(0x582a ,0x9 );
Camera_WriteRegister(0x582b ,0x6 );
Camera_WriteRegister(0x582c ,0x5 );
Camera_WriteRegister(0x582d ,0x6 );
Camera_WriteRegister(0x582e ,0xa );
Camera_WriteRegister(0x582f ,0xe );
Camera_WriteRegister(0x5830 ,0x28);
Camera_WriteRegister(0x5831 ,0x1a);
Camera_WriteRegister(0x5832 ,0x11);
Camera_WriteRegister(0x5833 ,0xe );
Camera_WriteRegister(0x5834 ,0xe );
Camera_WriteRegister(0x5835 ,0xf );
Camera_WriteRegister(0x5836 ,0x15);
Camera_WriteRegister(0x5837 ,0x1d);
Camera_WriteRegister(0x5838 ,0x6e);
Camera_WriteRegister(0x5839 ,0x39);
Camera_WriteRegister(0x583a ,0x27);
Camera_WriteRegister(0x583b ,0x1f);
Camera_WriteRegister(0x583c ,0x1e);
Camera_WriteRegister(0x583d ,0x23);
Camera_WriteRegister(0x583e ,0x2f);
Camera_WriteRegister(0x583f ,0x41);
Camera_WriteRegister(0x5840 ,0xe );
Camera_WriteRegister(0x5841 ,0xc );
Camera_WriteRegister(0x5842 ,0xd );
Camera_WriteRegister(0x5843 ,0xc );
Camera_WriteRegister(0x5844 ,0xc );
Camera_WriteRegister(0x5845 ,0xc );
Camera_WriteRegister(0x5846 ,0xc );
Camera_WriteRegister(0x5847 ,0xc );
Camera_WriteRegister(0x5848 ,0xd );
Camera_WriteRegister(0x5849 ,0xe );
Camera_WriteRegister(0x584a ,0xe );
Camera_WriteRegister(0x584b ,0xa );
Camera_WriteRegister(0x584c ,0xe );
Camera_WriteRegister(0x584d ,0xe );
Camera_WriteRegister(0x584e ,0x10);
Camera_WriteRegister(0x584f ,0x10);
Camera_WriteRegister(0x5850 ,0x11);
Camera_WriteRegister(0x5851 ,0xa );
Camera_WriteRegister(0x5852 ,0xf );
Camera_WriteRegister(0x5853 ,0xe );
Camera_WriteRegister(0x5854 ,0x10);
Camera_WriteRegister(0x5855 ,0x10);
Camera_WriteRegister(0x5856 ,0x10);
Camera_WriteRegister(0x5857 ,0xa );
Camera_WriteRegister(0x5858 ,0xe );
Camera_WriteRegister(0x5859 ,0xe );
Camera_WriteRegister(0x585a ,0xf );
Camera_WriteRegister(0x585b ,0xf );
Camera_WriteRegister(0x585c ,0xf );
Camera_WriteRegister(0x585d ,0xa );
Camera_WriteRegister(0x585e ,0x9 );
Camera_WriteRegister(0x585f ,0xd );
Camera_WriteRegister(0x5860 ,0xc );
Camera_WriteRegister(0x5861 ,0xb );
Camera_WriteRegister(0x5862 ,0xd );
Camera_WriteRegister(0x5863 ,0x7 );
Camera_WriteRegister(0x5864 ,0x17);
Camera_WriteRegister(0x5865 ,0x14);
Camera_WriteRegister(0x5866 ,0x18);
Camera_WriteRegister(0x5867 ,0x18);
Camera_WriteRegister(0x5868 ,0x16);
Camera_WriteRegister(0x5869 ,0x12);
Camera_WriteRegister(0x586a ,0x1b);
Camera_WriteRegister(0x586b ,0x1a);
Camera_WriteRegister(0x586c ,0x16);
Camera_WriteRegister(0x586d ,0x16);
Camera_WriteRegister(0x586e ,0x18);
Camera_WriteRegister(0x586f ,0x1f);
Camera_WriteRegister(0x5870 ,0x1c);
Camera_WriteRegister(0x5871 ,0x16);
Camera_WriteRegister(0x5872 ,0x10);
Camera_WriteRegister(0x5873 ,0xf );
Camera_WriteRegister(0x5874 ,0x13);
Camera_WriteRegister(0x5875 ,0x1c);
Camera_WriteRegister(0x5876 ,0x1e);
Camera_WriteRegister(0x5877 ,0x17);
Camera_WriteRegister(0x5878 ,0x11);
Camera_WriteRegister(0x5879 ,0x11);
Camera_WriteRegister(0x587a ,0x14);
Camera_WriteRegister(0x587b ,0x1e);
Camera_WriteRegister(0x587c ,0x1c);
Camera_WriteRegister(0x587d ,0x1c);
Camera_WriteRegister(0x587e ,0x1a);
Camera_WriteRegister(0x587f ,0x1a);
Camera_WriteRegister(0x5880 ,0x1b);
Camera_WriteRegister(0x5881 ,0x1f);
Camera_WriteRegister(0x5882 ,0x14);
Camera_WriteRegister(0x5883 ,0x1a);
Camera_WriteRegister(0x5884 ,0x1d);
Camera_WriteRegister(0x5885 ,0x1e);
Camera_WriteRegister(0x5886 ,0x1a);
Camera_WriteRegister(0x5887 ,0x1a);
//AWB
Camera_WriteRegister(0x5180 ,0xff);
Camera_WriteRegister(0x5181 ,0x52);
Camera_WriteRegister(0x5182 ,0x11);
Camera_WriteRegister(0x5183 ,0x14);
Camera_WriteRegister(0x5184 ,0x25);
Camera_WriteRegister(0x5185 ,0x24);
Camera_WriteRegister(0x5186 ,0x14);
Camera_WriteRegister(0x5187 ,0x14);
Camera_WriteRegister(0x5188 ,0x14);
Camera_WriteRegister(0x5189 ,0x69);
Camera_WriteRegister(0x518a ,0x60);
Camera_WriteRegister(0x518b ,0xa2);
Camera_WriteRegister(0x518c ,0x9c);
Camera_WriteRegister(0x518d ,0x36);
Camera_WriteRegister(0x518e ,0x34);
Camera_WriteRegister(0x518f ,0x54);
Camera_WriteRegister(0x5190 ,0x4c);
Camera_WriteRegister(0x5191 ,0xf8);
Camera_WriteRegister(0x5192 ,0x04);
Camera_WriteRegister(0x5193 ,0x70);
Camera_WriteRegister(0x5194 ,0xf0);
Camera_WriteRegister(0x5195 ,0xf0);
Camera_WriteRegister(0x5196 ,0x03);
Camera_WriteRegister(0x5197 ,0x01);
Camera_WriteRegister(0x5198 ,0x05);
Camera_WriteRegister(0x5199 ,0x2f);
Camera_WriteRegister(0x519a ,0x04);
Camera_WriteRegister(0x519b ,0x00);
Camera_WriteRegister(0x519c ,0x06);
Camera_WriteRegister(0x519d ,0xa0);
Camera_WriteRegister(0x519e ,0xa0);
//D/S
Camera_WriteRegister(0x528a ,0x00);
Camera_WriteRegister(0x528b ,0x01);
Camera_WriteRegister(0x528c ,0x04);
Camera_WriteRegister(0x528d ,0x08);
Camera_WriteRegister(0x528e ,0x10);
Camera_WriteRegister(0x528f ,0x20);
Camera_WriteRegister(0x5290 ,0x30);
Camera_WriteRegister(0x5292 ,0x00);
Camera_WriteRegister(0x5293 ,0x00);
Camera_WriteRegister(0x5294 ,0x00);
Camera_WriteRegister(0x5295 ,0x01);
Camera_WriteRegister(0x5296 ,0x00);
Camera_WriteRegister(0x5297 ,0x04);
Camera_WriteRegister(0x5298 ,0x00);
Camera_WriteRegister(0x5299 ,0x08);
Camera_WriteRegister(0x529a ,0x00);
Camera_WriteRegister(0x529b ,0x10);
Camera_WriteRegister(0x529c ,0x00);
Camera_WriteRegister(0x529d ,0x20);
Camera_WriteRegister(0x529e ,0x00);
Camera_WriteRegister(0x529f ,0x30);
Camera_WriteRegister(0x5282 ,0x00);
Camera_WriteRegister(0x5300 ,0x00);
Camera_WriteRegister(0x5301 ,0x20);
Camera_WriteRegister(0x5302 ,0x00);
Camera_WriteRegister(0x5303 ,0x7c);
Camera_WriteRegister(0x530c ,0x00);
Camera_WriteRegister(0x530d ,0x10);
Camera_WriteRegister(0x530e ,0x20);
Camera_WriteRegister(0x530f ,0x80);
Camera_WriteRegister(0x5310 ,0x20);
Camera_WriteRegister(0x5311 ,0x80);
Camera_WriteRegister(0x5308 ,0x20);
Camera_WriteRegister(0x5309 ,0x40);
Camera_WriteRegister(0x5304 ,0x00);
Camera_WriteRegister(0x5305 ,0x30);
Camera_WriteRegister(0x5306 ,0x00);
Camera_WriteRegister(0x5307 ,0x80);
Camera_WriteRegister(0x5314 ,0x08);
Camera_WriteRegister(0x5315 ,0x20);
Camera_WriteRegister(0x5319 ,0x30);
Camera_WriteRegister(0x5316 ,0x10);
Camera_WriteRegister(0x5317 ,0x00);
Camera_WriteRegister(0x5318 ,0x02);
//CMX
Camera_WriteRegister(0x5380 ,0x01);
Camera_WriteRegister(0x5381 ,0x00);
Camera_WriteRegister(0x5382 ,0x00);
Camera_WriteRegister(0x5383 ,0x1f);
Camera_WriteRegister(0x5384 ,0x00);
Camera_WriteRegister(0x5385 ,0x06);
Camera_WriteRegister(0x5386 ,0x00);
Camera_WriteRegister(0x5387 ,0x00);
Camera_WriteRegister(0x5388 ,0x00);
Camera_WriteRegister(0x5389 ,0xE1);
Camera_WriteRegister(0x538A ,0x00);
Camera_WriteRegister(0x538B ,0x2B);
Camera_WriteRegister(0x538C ,0x00);
Camera_WriteRegister(0x538D ,0x00);
Camera_WriteRegister(0x538E ,0x00);
Camera_WriteRegister(0x538F ,0x10);
Camera_WriteRegister(0x5390 ,0x00);
Camera_WriteRegister(0x5391 ,0xB3);
Camera_WriteRegister(0x5392 ,0x00);
Camera_WriteRegister(0x5393 ,0xA6);
Camera_WriteRegister(0x5394 ,0x08);
//GAMMA
Camera_WriteRegister(0x5480 ,0x0c);
Camera_WriteRegister(0x5481 ,0x18);
Camera_WriteRegister(0x5482 ,0x2f);
Camera_WriteRegister(0x5483 ,0x55);
Camera_WriteRegister(0x5484 ,0x64);
Camera_WriteRegister(0x5485 ,0x71);
Camera_WriteRegister(0x5486 ,0x7d);
Camera_WriteRegister(0x5487 ,0x87);
Camera_WriteRegister(0x5488 ,0x91);
Camera_WriteRegister(0x5489 ,0x9a);
Camera_WriteRegister(0x548A ,0xaa);
Camera_WriteRegister(0x548B ,0xb8);
Camera_WriteRegister(0x548C ,0xcd);
Camera_WriteRegister(0x548D ,0xdd);
Camera_WriteRegister(0x548E ,0xea);
Camera_WriteRegister(0x548F ,0x1d);
Camera_WriteRegister(0x5490 ,0x05);
Camera_WriteRegister(0x5491 ,0x00);
Camera_WriteRegister(0x5492 ,0x04);
Camera_WriteRegister(0x5493 ,0x20);
Camera_WriteRegister(0x5494 ,0x03);
Camera_WriteRegister(0x5495 ,0x60);
Camera_WriteRegister(0x5496 ,0x02);
Camera_WriteRegister(0x5497 ,0xB8);
Camera_WriteRegister(0x5498 ,0x02);
Camera_WriteRegister(0x5499 ,0x86);
Camera_WriteRegister(0x549A ,0x02);
Camera_WriteRegister(0x549B ,0x5B);
Camera_WriteRegister(0x549C ,0x02);
Camera_WriteRegister(0x549D ,0x3B);
Camera_WriteRegister(0x549E ,0x02);
Camera_WriteRegister(0x549F ,0x1C);
Camera_WriteRegister(0x54A0 ,0x02);
Camera_WriteRegister(0x54A1 ,0x04);
Camera_WriteRegister(0x54A2 ,0x01);
Camera_WriteRegister(0x54A3 ,0xED);
Camera_WriteRegister(0x54A4 ,0x01);
Camera_WriteRegister(0x54A5 ,0xC5);
Camera_WriteRegister(0x54A6 ,0x01);
Camera_WriteRegister(0x54A7 ,0xA5);
Camera_WriteRegister(0x54A8 ,0x01);
Camera_WriteRegister(0x54A9 ,0x6C);
Camera_WriteRegister(0x54AA ,0x01);
Camera_WriteRegister(0x54AB ,0x41);
Camera_WriteRegister(0x54AC ,0x01);
Camera_WriteRegister(0x54AD ,0x20);
Camera_WriteRegister(0x54AE ,0x00);
Camera_WriteRegister(0x54AF ,0x16);
Camera_WriteRegister(0x54B0 ,0x01);
Camera_WriteRegister(0x54B1 ,0x20);
Camera_WriteRegister(0x54B2 ,0x00);
Camera_WriteRegister(0x54B3 ,0x10);
Camera_WriteRegister(0x54B4 ,0x00);
Camera_WriteRegister(0x54B5 ,0xf0);
Camera_WriteRegister(0x54B6 ,0x00);
Camera_WriteRegister(0x54B7 ,0xDF);
Camera_WriteRegister(0x5402 ,0x3f);
Camera_WriteRegister(0x5403 ,0x00);
//UV ADJUST
Camera_WriteRegister(0x5500 ,0x10);
Camera_WriteRegister(0x5502 ,0x00);
Camera_WriteRegister(0x5503 ,0x06);
Camera_WriteRegister(0x5504 ,0x00);
Camera_WriteRegister(0x5505 ,0x7f);
//AE
Camera_WriteRegister(0x5025 ,0x80);
Camera_WriteRegister(0x3a0f ,0x30);
Camera_WriteRegister(0x3a10 ,0x28);
Camera_WriteRegister(0x3a1b ,0x30);
Camera_WriteRegister(0x3a1e ,0x28);
Camera_WriteRegister(0x3a11 ,0x61);
Camera_WriteRegister(0x3a1f ,0x10);
Camera_WriteRegister(0x5688 ,0xfd);
Camera_WriteRegister(0x5689 ,0xdf);
Camera_WriteRegister(0x568a ,0xfe);
Camera_WriteRegister(0x568b ,0xef);
Camera_WriteRegister(0x568c ,0xfe);
Camera_WriteRegister(0x568d ,0xef);
Camera_WriteRegister(0x568e ,0xaa);
Camera_WriteRegister(0x568f ,0xaa);	

Camera_WriteRegister(0x3815 ,0x01);//1600.1200
Camera_WriteRegister(0x4300 ,0x32);//YUV 422, UYVY 설정
Camera_WriteRegister(0x4730,0x01);//CCIR656 MODE ENABLE

}
void High_Resolution_Video_1080P( void )
{
	Camera_WriteRegister(0x3103 ,0x93);
	Camera_WriteRegister(0x3008 ,0x82);
	Camera_WriteRegister(0x3017 ,0x7f);
	Camera_WriteRegister(0x3018 ,0xfc);
	Camera_WriteRegister(0x3810 ,0xc2);
	Camera_WriteRegister(0x3615 ,0xf0);
	Camera_WriteRegister(0x3000 ,0x00);
	Camera_WriteRegister(0x3001 ,0x00);
	Camera_WriteRegister(0x3002 ,0x00);
	Camera_WriteRegister(0x3003 ,0x00);
	Camera_WriteRegister(0x3004 ,0xff);
	
	Camera_WriteRegister(0x3030 ,0x2b);
	Camera_WriteRegister(0x3011 ,0x08);
	Camera_WriteRegister(0x3010 ,0x10);
	
	Camera_WriteRegister(0x3604 ,0x60);
	Camera_WriteRegister(0x3622 ,0x60);
	Camera_WriteRegister(0x3621 ,0x09);
	Camera_WriteRegister(0x3709 ,0x00);
	Camera_WriteRegister(0x4000 ,0x21);
	Camera_WriteRegister(0x401d ,0x22);
	Camera_WriteRegister(0x3600 ,0x54);
	Camera_WriteRegister(0x3605 ,0x04);
	Camera_WriteRegister(0x3606 ,0x3f);
	Camera_WriteRegister(0x3c01 ,0x80);
	Camera_WriteRegister(0x300d ,0x22);
	Camera_WriteRegister(0x3623 ,0x22);
	Camera_WriteRegister(0x5000 ,0x4f);
	Camera_WriteRegister(0x5020 ,0x04);
	Camera_WriteRegister(0x5181 ,0x79);
	Camera_WriteRegister(0x5182 ,0x00);
	Camera_WriteRegister(0x5185 ,0x22);
	Camera_WriteRegister(0x5197 ,0x01);
	Camera_WriteRegister(0x5500 ,0x0a);
	Camera_WriteRegister(0x5504 ,0x00);
	Camera_WriteRegister(0x5505 ,0x7f);
	Camera_WriteRegister(0x5080 ,0x08);
	Camera_WriteRegister(0x300e ,0x18);
	Camera_WriteRegister(0x4610 ,0x00);
	Camera_WriteRegister(0x471d ,0x05);
	Camera_WriteRegister(0x4708 ,0x06);
	Camera_WriteRegister(0x370c ,0xa0);
	
	Camera_WriteRegister(0x3808 ,0x0a);
	Camera_WriteRegister(0x3809 ,0x20);
	Camera_WriteRegister(0x380a ,0x07);
	Camera_WriteRegister(0x380b ,0x98);
	Camera_WriteRegister(0x380c ,0x0c);
	Camera_WriteRegister(0x380d ,0x80);
	Camera_WriteRegister(0x380e ,0x07);
	
	Camera_WriteRegister(0x380f ,0xd0);
	Camera_WriteRegister(0x5687 ,0x94);
	Camera_WriteRegister(0x501f ,0x00);
	Camera_WriteRegister(0x5000 ,0x4f);
	Camera_WriteRegister(0x5001 ,0xcf);
	Camera_WriteRegister(0x4300 ,0x30);
	Camera_WriteRegister(0x4300 ,0x30);
	Camera_WriteRegister(0x460b ,0x35);
	Camera_WriteRegister(0x471d ,0x00);
	Camera_WriteRegister(0x3002 ,0x0c);
	Camera_WriteRegister(0x3002 ,0x00);
	Camera_WriteRegister(0x4713 ,0x03);
	Camera_WriteRegister(0x471c ,0x50);
	Camera_WriteRegister(0x4721 ,0x02);
	Camera_WriteRegister(0x4402 ,0x90);
	Camera_WriteRegister(0x460c ,0x22);
	Camera_WriteRegister(0x3815 ,0x44);
	Camera_WriteRegister(0x3503 ,0x07);
	Camera_WriteRegister(0x3501 ,0x73);
	Camera_WriteRegister(0x3502 ,0x80);
	Camera_WriteRegister(0x350b ,0x00);
	Camera_WriteRegister(0x3818 ,0xc8);
	Camera_WriteRegister(0x3801 ,0x88);
	Camera_WriteRegister(0x3824 ,0x11);
	Camera_WriteRegister(0x3a00 ,0x78);
	Camera_WriteRegister(0x3a1a ,0x04);
	Camera_WriteRegister(0x3a13 ,0x30);
	Camera_WriteRegister(0x3a18 ,0x00);
	Camera_WriteRegister(0x3a19 ,0x7c);
	Camera_WriteRegister(0x3a08 ,0x12);
	Camera_WriteRegister(0x3a09 ,0xc0);
	Camera_WriteRegister(0x3a0a ,0x0f);
	Camera_WriteRegister(0x3a0b ,0xa0);
	Camera_WriteRegister(0x350c ,0x07);
	Camera_WriteRegister(0x350d ,0xd0);
	Camera_WriteRegister(0x3a0d ,0x08);
	Camera_WriteRegister(0x3a0e ,0x06);
	Camera_WriteRegister(0x3500 ,0x00);
	Camera_WriteRegister(0x3501 ,0x00);
	Camera_WriteRegister(0x3502 ,0x00);
	Camera_WriteRegister(0x350a ,0x00);
	Camera_WriteRegister(0x350b ,0x00);
	Camera_WriteRegister(0x3503 ,0x00);
	Camera_WriteRegister(0x3030 ,0x2b);
	Camera_WriteRegister(0x3a02 ,0x00);
	Camera_WriteRegister(0x3a03 ,0x7d);
	Camera_WriteRegister(0x3a04 ,0x00);
	Camera_WriteRegister(0x3a14 ,0x00);
	Camera_WriteRegister(0x3a15 ,0x7d);
	Camera_WriteRegister(0x3a16 ,0x00);
	Camera_WriteRegister(0x3a00 ,0x78);
	Camera_WriteRegister(0x3a08 ,0x09);
	Camera_WriteRegister(0x3a09 ,0x60);
	Camera_WriteRegister(0x3a0a ,0x07);
	Camera_WriteRegister(0x3a0b ,0xd0);
	Camera_WriteRegister(0x3a0d ,0x10);
	Camera_WriteRegister(0x3a0e ,0x0d);
	Camera_WriteRegister(0x4407 ,0x04);
	Camera_WriteRegister(0x5193 ,0x70);
	Camera_WriteRegister(0x589b ,0x00);
	Camera_WriteRegister(0x589a ,0xc0);
	Camera_WriteRegister(0x401e ,0x20);
	Camera_WriteRegister(0x4001 ,0x42);
	Camera_WriteRegister(0x401c ,0x06);
	Camera_WriteRegister(0x3825 ,0xac);
	Camera_WriteRegister(0x3827 ,0x0c);
	Camera_WriteRegister(0x5402 ,0x3f);
	Camera_WriteRegister(0x5403 ,0x00);
	Camera_WriteRegister(0x3406 ,0x00);
	Camera_WriteRegister(0x5180 ,0xff);
	Camera_WriteRegister(0x5181 ,0x52);
	Camera_WriteRegister(0x5182 ,0x11);
	Camera_WriteRegister(0x5183 ,0x14);
	Camera_WriteRegister(0x5184 ,0x25);
	Camera_WriteRegister(0x5185 ,0x24);
	Camera_WriteRegister(0x5186 ,0x06);
	Camera_WriteRegister(0x5187 ,0x08);
	Camera_WriteRegister(0x5188 ,0x08);
	Camera_WriteRegister(0x5189 ,0x7c);
	Camera_WriteRegister(0x518a ,0x60);
	Camera_WriteRegister(0x518b ,0xb2);
	Camera_WriteRegister(0x518c ,0xb2);
	Camera_WriteRegister(0x518d ,0x44);
	Camera_WriteRegister(0x518e ,0x3d);
	Camera_WriteRegister(0x518f ,0x58);
	Camera_WriteRegister(0x5190 ,0x46);
	Camera_WriteRegister(0x5191 ,0xf8);
	Camera_WriteRegister(0x5192 ,0x04);
	Camera_WriteRegister(0x5193 ,0x70);
	Camera_WriteRegister(0x5194 ,0xf0);
	Camera_WriteRegister(0x5195 ,0xf0);
	Camera_WriteRegister(0x5196 ,0x03);
	Camera_WriteRegister(0x5197 ,0x01);
	Camera_WriteRegister(0x5198 ,0x04);
	Camera_WriteRegister(0x5199 ,0x12);
	Camera_WriteRegister(0x519a ,0x04);
	Camera_WriteRegister(0x519b ,0x00);
	Camera_WriteRegister(0x519c ,0x06);
	Camera_WriteRegister(0x519d ,0x82);
	Camera_WriteRegister(0x519e ,0x00);
	Camera_WriteRegister(0x5025 ,0x80);
	Camera_WriteRegister(0x5583 ,0x40);
	Camera_WriteRegister(0x5584 ,0x40);
	Camera_WriteRegister(0x5580 ,0x02);
	Camera_WriteRegister(0x5000 ,0xcf);
	Camera_WriteRegister(0x3710 ,0x10);
	Camera_WriteRegister(0x3632 ,0x51);
	Camera_WriteRegister(0x3702 ,0x10);
	Camera_WriteRegister(0x3703 ,0xb2);
	Camera_WriteRegister(0x3704 ,0x18);
	Camera_WriteRegister(0x370b ,0x40);
	Camera_WriteRegister(0x370d ,0x03);
	Camera_WriteRegister(0x3631 ,0x01);
	Camera_WriteRegister(0x3632 ,0x52);
	Camera_WriteRegister(0x3606 ,0x24);
	Camera_WriteRegister(0x3620 ,0x96);
	Camera_WriteRegister(0x5785 ,0x07);
	Camera_WriteRegister(0x3a13 ,0x30);
	Camera_WriteRegister(0x3600 ,0x52);
	Camera_WriteRegister(0x3604 ,0x48);
	Camera_WriteRegister(0x3606 ,0x1b);
	Camera_WriteRegister(0x370d ,0x0b);
	Camera_WriteRegister(0x370f ,0xc0);
	Camera_WriteRegister(0x3709 ,0x01);
	Camera_WriteRegister(0x3823 ,0x00);
	Camera_WriteRegister(0x5007 ,0x00);
	Camera_WriteRegister(0x5009 ,0x00);
	Camera_WriteRegister(0x5011 ,0x00);
	Camera_WriteRegister(0x5013 ,0x00);
	Camera_WriteRegister(0x519e ,0x00);
	Camera_WriteRegister(0x5086 ,0x00);
	Camera_WriteRegister(0x5087 ,0x00);
	Camera_WriteRegister(0x5088 ,0x00);
	Camera_WriteRegister(0x5089 ,0x00);
	Camera_WriteRegister(0x302b ,0x00);
	Camera_WriteRegister(0x3503 ,0x07);
	Camera_WriteRegister(0x3011 ,0x07);
	Camera_WriteRegister(0x350c ,0x04);
	Camera_WriteRegister(0x350d ,0x58);
	
	Camera_WriteRegister(0x3801 ,0x8a);
	Camera_WriteRegister(0x3803 ,0x0a);
	Camera_WriteRegister(0x3804 ,0x07);
	Camera_WriteRegister(0x3805 ,0x80);
	Camera_WriteRegister(0x3806 ,0x04);
	Camera_WriteRegister(0x3807 ,0x38);
	Camera_WriteRegister(0x3808 ,0x07);
	Camera_WriteRegister(0x3809 ,0x80);
	Camera_WriteRegister(0x380a ,0x04);
	Camera_WriteRegister(0x380b ,0x38);
	Camera_WriteRegister(0x380c ,0x09);
	Camera_WriteRegister(0x380d ,0xd6);
	Camera_WriteRegister(0x380e ,0x04);
	Camera_WriteRegister(0x380f ,0x58);
	
	Camera_WriteRegister(0x381c ,0x11);
	Camera_WriteRegister(0x381d ,0xba);
	Camera_WriteRegister(0x381e ,0x04);
	Camera_WriteRegister(0x381f ,0x48);
	Camera_WriteRegister(0x3820 ,0x04);
	Camera_WriteRegister(0x3821 ,0x18);
	Camera_WriteRegister(0x3a08 ,0x14);
	Camera_WriteRegister(0x3a09 ,0xe0);
	Camera_WriteRegister(0x3a0a ,0x11);
	Camera_WriteRegister(0x3a0b ,0x60);
	Camera_WriteRegister(0x3a0d ,0x04);
	Camera_WriteRegister(0x3a0e ,0x03);
	Camera_WriteRegister(0x5682 ,0x07);
	Camera_WriteRegister(0x5683 ,0x60);
	Camera_WriteRegister(0x5686 ,0x04);
	Camera_WriteRegister(0x5687 ,0x1c);
	Camera_WriteRegister(0x5001 ,0x7f);
	Camera_WriteRegister(0x3503 ,0x00);
	Camera_WriteRegister(0x3010 ,0x10);
	Camera_WriteRegister(0x5001 ,0xFF);
	Camera_WriteRegister(0x5583 ,0x50);
	Camera_WriteRegister(0x5584 ,0x50);
	Camera_WriteRegister(0x5580 ,0x02);
	Camera_WriteRegister(0x3c01 ,0x80);
	Camera_WriteRegister(0x3c00 ,0x04);
	//LENS
	Camera_WriteRegister(0x5800 ,0x48);
	Camera_WriteRegister(0x5801 ,0x31);
	Camera_WriteRegister(0x5802 ,0x21);
	Camera_WriteRegister(0x5803 ,0x1b);
	Camera_WriteRegister(0x5804 ,0x1a);
	Camera_WriteRegister(0x5805 ,0x1e);
	Camera_WriteRegister(0x5806 ,0x29);
	Camera_WriteRegister(0x5807 ,0x38);
	Camera_WriteRegister(0x5808 ,0x26);
	Camera_WriteRegister(0x5809 ,0x17);
	Camera_WriteRegister(0x580a ,0x11);
	Camera_WriteRegister(0x580b ,0xe );
	Camera_WriteRegister(0x580c ,0xd );
	Camera_WriteRegister(0x580d ,0xe );
	Camera_WriteRegister(0x580e ,0x13);
	Camera_WriteRegister(0x580f ,0x1a);
	Camera_WriteRegister(0x5810 ,0x15);
	Camera_WriteRegister(0x5811 ,0xd );
	Camera_WriteRegister(0x5812 ,0x8 );
	Camera_WriteRegister(0x5813 ,0x5 );
	Camera_WriteRegister(0x5814 ,0x4 );
	Camera_WriteRegister(0x5815 ,0x5 );
	Camera_WriteRegister(0x5816 ,0x9 );
	Camera_WriteRegister(0x5817 ,0xd );
	Camera_WriteRegister(0x5818 ,0x11);
	Camera_WriteRegister(0x5819 ,0xa );
	Camera_WriteRegister(0x581a ,0x4 );
	Camera_WriteRegister(0x581b ,0x0 );
	Camera_WriteRegister(0x581c ,0x0 );
	Camera_WriteRegister(0x581d ,0x1 );
	Camera_WriteRegister(0x581e ,0x6 );
	Camera_WriteRegister(0x581f ,0x9 );
	Camera_WriteRegister(0x5820 ,0x12);
	Camera_WriteRegister(0x5821 ,0xb );
	Camera_WriteRegister(0x5822 ,0x4 );
	Camera_WriteRegister(0x5823 ,0x0 );
	Camera_WriteRegister(0x5824 ,0x0 );
	Camera_WriteRegister(0x5825 ,0x1 );
	Camera_WriteRegister(0x5826 ,0x6 );
	Camera_WriteRegister(0x5827 ,0xa );
	Camera_WriteRegister(0x5828 ,0x17);
	Camera_WriteRegister(0x5829 ,0xf );
	Camera_WriteRegister(0x582a ,0x9 );
	Camera_WriteRegister(0x582b ,0x6 );
	Camera_WriteRegister(0x582c ,0x5 );
	Camera_WriteRegister(0x582d ,0x6 );
	Camera_WriteRegister(0x582e ,0xa );
	Camera_WriteRegister(0x582f ,0xe );
	Camera_WriteRegister(0x5830 ,0x28);
	Camera_WriteRegister(0x5831 ,0x1a);
	Camera_WriteRegister(0x5832 ,0x11);
	Camera_WriteRegister(0x5833 ,0xe );
	Camera_WriteRegister(0x5834 ,0xe );
	Camera_WriteRegister(0x5835 ,0xf );
	Camera_WriteRegister(0x5836 ,0x15);
	Camera_WriteRegister(0x5837 ,0x1d);
	Camera_WriteRegister(0x5838 ,0x6e);
	Camera_WriteRegister(0x5839 ,0x39);
	Camera_WriteRegister(0x583a ,0x27);
	Camera_WriteRegister(0x583b ,0x1f);
	Camera_WriteRegister(0x583c ,0x1e);
	Camera_WriteRegister(0x583d ,0x23);
	Camera_WriteRegister(0x583e ,0x2f);
	Camera_WriteRegister(0x583f ,0x41);
	Camera_WriteRegister(0x5840 ,0xe );
	Camera_WriteRegister(0x5841 ,0xc );
	Camera_WriteRegister(0x5842 ,0xd );
	Camera_WriteRegister(0x5843 ,0xc );
	Camera_WriteRegister(0x5844 ,0xc );
	Camera_WriteRegister(0x5845 ,0xc );
	Camera_WriteRegister(0x5846 ,0xc );
	Camera_WriteRegister(0x5847 ,0xc );
	Camera_WriteRegister(0x5848 ,0xd );
	Camera_WriteRegister(0x5849 ,0xe );
	Camera_WriteRegister(0x584a ,0xe );
	Camera_WriteRegister(0x584b ,0xa );
	Camera_WriteRegister(0x584c ,0xe );
	Camera_WriteRegister(0x584d ,0xe );
	Camera_WriteRegister(0x584e ,0x10);
	Camera_WriteRegister(0x584f ,0x10);
	Camera_WriteRegister(0x5850 ,0x11);
	Camera_WriteRegister(0x5851 ,0xa );
	Camera_WriteRegister(0x5852 ,0xf );
	Camera_WriteRegister(0x5853 ,0xe );
	Camera_WriteRegister(0x5854 ,0x10);
	Camera_WriteRegister(0x5855 ,0x10);
	Camera_WriteRegister(0x5856 ,0x10);
	Camera_WriteRegister(0x5857 ,0xa );
	Camera_WriteRegister(0x5858 ,0xe );
	Camera_WriteRegister(0x5859 ,0xe );
	Camera_WriteRegister(0x585a ,0xf );
	Camera_WriteRegister(0x585b ,0xf );
	Camera_WriteRegister(0x585c ,0xf );
	Camera_WriteRegister(0x585d ,0xa );
	Camera_WriteRegister(0x585e ,0x9 );
	Camera_WriteRegister(0x585f ,0xd );
	Camera_WriteRegister(0x5860 ,0xc );
	Camera_WriteRegister(0x5861 ,0xb );
	Camera_WriteRegister(0x5862 ,0xd );
	Camera_WriteRegister(0x5863 ,0x7 );
	Camera_WriteRegister(0x5864 ,0x17);
	Camera_WriteRegister(0x5865 ,0x14);
	Camera_WriteRegister(0x5866 ,0x18);
	Camera_WriteRegister(0x5867 ,0x18);
	Camera_WriteRegister(0x5868 ,0x16);
	Camera_WriteRegister(0x5869 ,0x12);
	Camera_WriteRegister(0x586a ,0x1b);
	Camera_WriteRegister(0x586b ,0x1a);
	Camera_WriteRegister(0x586c ,0x16);
	Camera_WriteRegister(0x586d ,0x16);
	Camera_WriteRegister(0x586e ,0x18);
	Camera_WriteRegister(0x586f ,0x1f);
	Camera_WriteRegister(0x5870 ,0x1c);
	Camera_WriteRegister(0x5871 ,0x16);
	Camera_WriteRegister(0x5872 ,0x10);
	Camera_WriteRegister(0x5873 ,0xf );
	Camera_WriteRegister(0x5874 ,0x13);
	Camera_WriteRegister(0x5875 ,0x1c);
	Camera_WriteRegister(0x5876 ,0x1e);
	Camera_WriteRegister(0x5877 ,0x17);
	Camera_WriteRegister(0x5878 ,0x11);
	Camera_WriteRegister(0x5879 ,0x11);
	Camera_WriteRegister(0x587a ,0x14);
	Camera_WriteRegister(0x587b ,0x1e);
	Camera_WriteRegister(0x587c ,0x1c);
	Camera_WriteRegister(0x587d ,0x1c);
	Camera_WriteRegister(0x587e ,0x1a);
	Camera_WriteRegister(0x587f ,0x1a);
	Camera_WriteRegister(0x5880 ,0x1b);
	Camera_WriteRegister(0x5881 ,0x1f);
	Camera_WriteRegister(0x5882 ,0x14);
	Camera_WriteRegister(0x5883 ,0x1a);
	Camera_WriteRegister(0x5884 ,0x1d);
	Camera_WriteRegister(0x5885 ,0x1e);
	Camera_WriteRegister(0x5886 ,0x1a);
	Camera_WriteRegister(0x5887 ,0x1a);
	//AWB
	Camera_WriteRegister(0x5180 ,0xff);
	Camera_WriteRegister(0x5181 ,0x52);
	Camera_WriteRegister(0x5182 ,0x11);
	Camera_WriteRegister(0x5183 ,0x14);
	Camera_WriteRegister(0x5184 ,0x25);
	Camera_WriteRegister(0x5185 ,0x24);
	Camera_WriteRegister(0x5186 ,0x14);
	Camera_WriteRegister(0x5187 ,0x14);
	Camera_WriteRegister(0x5188 ,0x14);
	Camera_WriteRegister(0x5189 ,0x69);
	Camera_WriteRegister(0x518a ,0x60);
	Camera_WriteRegister(0x518b ,0xa2);
	Camera_WriteRegister(0x518c ,0x9c);
	Camera_WriteRegister(0x518d ,0x36);
	Camera_WriteRegister(0x518e ,0x34);
	Camera_WriteRegister(0x518f ,0x54);
	Camera_WriteRegister(0x5190 ,0x4c);
	Camera_WriteRegister(0x5191 ,0xf8);
	Camera_WriteRegister(0x5192 ,0x04);
	Camera_WriteRegister(0x5193 ,0x70);
	Camera_WriteRegister(0x5194 ,0xf0);
	Camera_WriteRegister(0x5195 ,0xf0);
	Camera_WriteRegister(0x5196 ,0x03);
	Camera_WriteRegister(0x5197 ,0x01);
	Camera_WriteRegister(0x5198 ,0x05);
	Camera_WriteRegister(0x5199 ,0x2f);
	Camera_WriteRegister(0x519a ,0x04);
	Camera_WriteRegister(0x519b ,0x00);
	Camera_WriteRegister(0x519c ,0x06);
	Camera_WriteRegister(0x519d ,0xa0);
	Camera_WriteRegister(0x519e ,0xa0);
	//D/S
	Camera_WriteRegister(0x528a ,0x00);
	Camera_WriteRegister(0x528b ,0x01);
	Camera_WriteRegister(0x528c ,0x04);
	Camera_WriteRegister(0x528d ,0x08);
	Camera_WriteRegister(0x528e ,0x10);
	Camera_WriteRegister(0x528f ,0x20);
	Camera_WriteRegister(0x5290 ,0x30);
	Camera_WriteRegister(0x5292 ,0x00);
	Camera_WriteRegister(0x5293 ,0x00);
	Camera_WriteRegister(0x5294 ,0x00);
	Camera_WriteRegister(0x5295 ,0x01);
	Camera_WriteRegister(0x5296 ,0x00);
	Camera_WriteRegister(0x5297 ,0x04);
	Camera_WriteRegister(0x5298 ,0x00);
	Camera_WriteRegister(0x5299 ,0x08);
	Camera_WriteRegister(0x529a ,0x00);
	Camera_WriteRegister(0x529b ,0x10);
	Camera_WriteRegister(0x529c ,0x00);
	Camera_WriteRegister(0x529d ,0x20);
	Camera_WriteRegister(0x529e ,0x00);
	Camera_WriteRegister(0x529f ,0x30);
	Camera_WriteRegister(0x5282 ,0x00);
	Camera_WriteRegister(0x5300 ,0x00);
	Camera_WriteRegister(0x5301 ,0x20);
	Camera_WriteRegister(0x5302 ,0x00);
	Camera_WriteRegister(0x5303 ,0x7c);
	Camera_WriteRegister(0x530c ,0x00);
	Camera_WriteRegister(0x530d ,0x10);
	Camera_WriteRegister(0x530e ,0x20);
	Camera_WriteRegister(0x530f ,0x80);
	Camera_WriteRegister(0x5310 ,0x20);
	Camera_WriteRegister(0x5311 ,0x80);
	Camera_WriteRegister(0x5308 ,0x20);
	Camera_WriteRegister(0x5309 ,0x40);
	Camera_WriteRegister(0x5304 ,0x00);
	Camera_WriteRegister(0x5305 ,0x30);
	Camera_WriteRegister(0x5306 ,0x00);
	Camera_WriteRegister(0x5307 ,0x80);
	Camera_WriteRegister(0x5314 ,0x08);
	Camera_WriteRegister(0x5315 ,0x20);
	Camera_WriteRegister(0x5319 ,0x30);
	Camera_WriteRegister(0x5316 ,0x10);
	Camera_WriteRegister(0x5317 ,0x00);
	Camera_WriteRegister(0x5318 ,0x02);
	//CMX
	Camera_WriteRegister(0x5380 ,0x01);
	Camera_WriteRegister(0x5381 ,0x00);
	Camera_WriteRegister(0x5382 ,0x00);
	Camera_WriteRegister(0x5383 ,0x1f);
	Camera_WriteRegister(0x5384 ,0x00);
	Camera_WriteRegister(0x5385 ,0x06);
	Camera_WriteRegister(0x5386 ,0x00);
	Camera_WriteRegister(0x5387 ,0x00);
	Camera_WriteRegister(0x5388 ,0x00);
	Camera_WriteRegister(0x5389 ,0xE1);
	Camera_WriteRegister(0x538A ,0x00);
	Camera_WriteRegister(0x538B ,0x2B);
	Camera_WriteRegister(0x538C ,0x00);
	Camera_WriteRegister(0x538D ,0x00);
	Camera_WriteRegister(0x538E ,0x00);
	Camera_WriteRegister(0x538F ,0x10);
	Camera_WriteRegister(0x5390 ,0x00);
	Camera_WriteRegister(0x5391 ,0xB3);
	Camera_WriteRegister(0x5392 ,0x00);
	Camera_WriteRegister(0x5393 ,0xA6);
	Camera_WriteRegister(0x5394 ,0x08);
	//GAMMA
	Camera_WriteRegister(0x5480 ,0x0c);
	Camera_WriteRegister(0x5481 ,0x18);
	Camera_WriteRegister(0x5482 ,0x2f);
	Camera_WriteRegister(0x5483 ,0x55);
	Camera_WriteRegister(0x5484 ,0x64);
	Camera_WriteRegister(0x5485 ,0x71);
	Camera_WriteRegister(0x5486 ,0x7d);
	Camera_WriteRegister(0x5487 ,0x87);
	Camera_WriteRegister(0x5488 ,0x91);
	Camera_WriteRegister(0x5489 ,0x9a);
	Camera_WriteRegister(0x548A ,0xaa);
	Camera_WriteRegister(0x548B ,0xb8);
	Camera_WriteRegister(0x548C ,0xcd);
	Camera_WriteRegister(0x548D ,0xdd);
	Camera_WriteRegister(0x548E ,0xea);
	Camera_WriteRegister(0x548F ,0x1d);
	Camera_WriteRegister(0x5490 ,0x05);
	Camera_WriteRegister(0x5491 ,0x00);
	Camera_WriteRegister(0x5492 ,0x04);
	Camera_WriteRegister(0x5493 ,0x20);
	Camera_WriteRegister(0x5494 ,0x03);
	Camera_WriteRegister(0x5495 ,0x60);
	Camera_WriteRegister(0x5496 ,0x02);
	Camera_WriteRegister(0x5497 ,0xB8);
	Camera_WriteRegister(0x5498 ,0x02);
	Camera_WriteRegister(0x5499 ,0x86);
	Camera_WriteRegister(0x549A ,0x02);
	Camera_WriteRegister(0x549B ,0x5B);
	Camera_WriteRegister(0x549C ,0x02);
	Camera_WriteRegister(0x549D ,0x3B);
	Camera_WriteRegister(0x549E ,0x02);
	Camera_WriteRegister(0x549F ,0x1C);
	Camera_WriteRegister(0x54A0 ,0x02);
	Camera_WriteRegister(0x54A1 ,0x04);
	Camera_WriteRegister(0x54A2 ,0x01);
	Camera_WriteRegister(0x54A3 ,0xED);
	Camera_WriteRegister(0x54A4 ,0x01);
	Camera_WriteRegister(0x54A5 ,0xC5);
	Camera_WriteRegister(0x54A6 ,0x01);
	Camera_WriteRegister(0x54A7 ,0xA5);
	Camera_WriteRegister(0x54A8 ,0x01);
	Camera_WriteRegister(0x54A9 ,0x6C);
	Camera_WriteRegister(0x54AA ,0x01);
	Camera_WriteRegister(0x54AB ,0x41);
	Camera_WriteRegister(0x54AC ,0x01);
	Camera_WriteRegister(0x54AD ,0x20);
	Camera_WriteRegister(0x54AE ,0x00);
	Camera_WriteRegister(0x54AF ,0x16);
	Camera_WriteRegister(0x54B0 ,0x01);
	Camera_WriteRegister(0x54B1 ,0x20);
	Camera_WriteRegister(0x54B2 ,0x00);
	Camera_WriteRegister(0x54B3 ,0x10);
	Camera_WriteRegister(0x54B4 ,0x00);
	Camera_WriteRegister(0x54B5 ,0xf0);
	Camera_WriteRegister(0x54B6 ,0x00);
	Camera_WriteRegister(0x54B7 ,0xDF);
	Camera_WriteRegister(0x5402 ,0x3f);
	Camera_WriteRegister(0x5403 ,0x00);
	//UV ADJUST
	Camera_WriteRegister(0x5500 ,0x10);
	Camera_WriteRegister(0x5502 ,0x00);
	Camera_WriteRegister(0x5503 ,0x06);
	Camera_WriteRegister(0x5504 ,0x00);
	Camera_WriteRegister(0x5505 ,0x7f);
	//AE
	Camera_WriteRegister(0x5025 ,0x80);
	Camera_WriteRegister(0x3a0f ,0x30);
	Camera_WriteRegister(0x3a10 ,0x28);
	Camera_WriteRegister(0x3a1b ,0x30);
	Camera_WriteRegister(0x3a1e ,0x28);
	Camera_WriteRegister(0x3a11 ,0x61);
	Camera_WriteRegister(0x3a1f ,0x10);
	Camera_WriteRegister(0x5688 ,0xfd);
	Camera_WriteRegister(0x5689 ,0xdf);
	Camera_WriteRegister(0x568a ,0xfe);
	Camera_WriteRegister(0x568b ,0xef);
	Camera_WriteRegister(0x568c ,0xfe);
	Camera_WriteRegister(0x568d ,0xef);
	Camera_WriteRegister(0x568e ,0xaa);
	Camera_WriteRegister(0x568f ,0xaa);		
	
	Camera_WriteRegister(0x4300 ,0x32);//YUV 422, UYVY 설정
	Camera_WriteRegister(0x4730,0x01);//CCIR656 MODE ENABLE
	
	Dprintf("HIGH RESOLUTION VIDEO INIT\n");
	
}

int Uxga1600_1200_Zoom( int zoom)
{
	if( zoom == 1)
	{
		Camera_WriteRegister(0x3800 ,0x1 );
		Camera_WriteRegister(0x3801 ,0x8A);
		Camera_WriteRegister(0x3802 ,0x0 );
		Camera_WriteRegister(0x3803 ,0xA );
		Camera_WriteRegister(0x3804 ,0xA );
		Camera_WriteRegister(0x3805 ,0x20);
		Camera_WriteRegister(0x3806 ,0x7 );
		Camera_WriteRegister(0x3807 ,0x98);
		Camera_WriteRegister(0x3808 ,0x6 );
		Camera_WriteRegister(0x3809 ,0x40);
		Camera_WriteRegister(0x380a ,0x4 );
		Camera_WriteRegister(0x380b ,0xb0);
		Camera_WriteRegister(0x380c ,0xc );
		Camera_WriteRegister(0x380d ,0x80);
		Camera_WriteRegister(0x380e ,0x7 );
		Camera_WriteRegister(0x380f ,0xd0);
		Camera_WriteRegister(0x5001 ,0x7f);
		Camera_WriteRegister(0x5680 ,0x0 );
		Camera_WriteRegister(0x5681 ,0x0 );
		Camera_WriteRegister(0x5682 ,0xA );
		Camera_WriteRegister(0x5683 ,0x20);
		Camera_WriteRegister(0x5684 ,0x0 );
		Camera_WriteRegister(0x5685 ,0x0 );
		Camera_WriteRegister(0x5686 ,0x7 );
		Camera_WriteRegister(0x5687 ,0x98);
	}
	if( zoom == 2 )
	{
		Camera_WriteRegister(0x3800 ,0x2 );
		Camera_WriteRegister(0x3801 ,0xe5);
		Camera_WriteRegister(0x3802 ,0x0 );
		Camera_WriteRegister(0x3803 ,0xeA );
		Camera_WriteRegister(0x3804 ,0x7 );
		Camera_WriteRegister(0x3805 ,0xc9);
		Camera_WriteRegister(0x3806 ,0x5 );
		Camera_WriteRegister(0x3807 ,0xd7);
		Camera_WriteRegister(0x3808 ,0x6 );
		Camera_WriteRegister(0x3809 ,0x40);
		Camera_WriteRegister(0x380a ,0x4 );
		Camera_WriteRegister(0x380b ,0xb0);
		Camera_WriteRegister(0x380c ,0xc );
		Camera_WriteRegister(0x380d ,0x80);
		Camera_WriteRegister(0x380e ,0x7 );
		Camera_WriteRegister(0x380f ,0xd0);
		Camera_WriteRegister(0x5001 ,0x7f);
		Camera_WriteRegister(0x5680 ,0x0 );
		Camera_WriteRegister(0x5681 ,0x0 );
		Camera_WriteRegister(0x5682 ,0x7 );
		Camera_WriteRegister(0x5683 ,0xc9);
		Camera_WriteRegister(0x5684 ,0x0 );
		Camera_WriteRegister(0x5685 ,0x0 );
		Camera_WriteRegister(0x5686 ,0x5 );
		Camera_WriteRegister(0x5687 ,0xd7);
	}
	if( zoom == 3 )
	{
		Camera_WriteRegister(0x3800 ,0x3 );
		Camera_WriteRegister(0x3801 ,0x70);
		Camera_WriteRegister(0x3802 ,0x1);
		Camera_WriteRegister(0x3803 ,0x76 );
		Camera_WriteRegister(0x3804 ,0x6 );
		Camera_WriteRegister(0x3805 ,0x54);
		Camera_WriteRegister(0x3806 ,0x4 );
		Camera_WriteRegister(0x3807 ,0xbf);
		Camera_WriteRegister(0x3808 ,0x6 );
		Camera_WriteRegister(0x3809 ,0x40);
		Camera_WriteRegister(0x380a ,0x4 );
		Camera_WriteRegister(0x380b ,0xb0);
		Camera_WriteRegister(0x380c ,0xc );
		Camera_WriteRegister(0x380d ,0x80);
		Camera_WriteRegister(0x380e ,0x7 );
		Camera_WriteRegister(0x380f ,0xd0);
		Camera_WriteRegister(0x5001 ,0x7f);
		Camera_WriteRegister(0x5680 ,0x0 );
		Camera_WriteRegister(0x5681 ,0x0 );
		Camera_WriteRegister(0x5682 ,0x6 );
		Camera_WriteRegister(0x5683 ,0x54);
		Camera_WriteRegister(0x5684 ,0x0 );
		Camera_WriteRegister(0x5685 ,0x0 );
		Camera_WriteRegister(0x5686 ,0x4 );
		Camera_WriteRegister(0x5687 ,0xbf);
	}
	return (0);
}



void OC5642_Sensor_RAW_Setting( void )
{
	Camera_WriteRegister(0x3103 ,0x03);
	Camera_WriteRegister(0x3008 ,0x82);
	Camera_WriteRegister(0x3017 ,0x7f);
	Camera_WriteRegister(0x3018 ,0xfc);
	Camera_WriteRegister(0x3810 ,0xc2);
	Camera_WriteRegister(0x3615 ,0xf0);
	Camera_WriteRegister(0x3000 ,0x00);
	Camera_WriteRegister(0x3001 ,0x00);
	Camera_WriteRegister(0x3002 ,0x00);
	Camera_WriteRegister(0x3003 ,0x00);
	Camera_WriteRegister(0x3011 ,0x08);
	Camera_WriteRegister(0x3010 ,0x30);
	Camera_WriteRegister(0x3604 ,0x60);
	Camera_WriteRegister(0x3622 ,0x08);
	Camera_WriteRegister(0x3621 ,0x17);
	Camera_WriteRegister(0x3709 ,0x00);
	Camera_WriteRegister(0x4000 ,0x21);
	Camera_WriteRegister(0x401d ,0x02);
	Camera_WriteRegister(0x3600 ,0x54);
	Camera_WriteRegister(0x3605 ,0x04);
	Camera_WriteRegister(0x3606 ,0x3f);
	Camera_WriteRegister(0x3c01 ,0x80);
	Camera_WriteRegister(0x300d ,0x21);
	Camera_WriteRegister(0x3623 ,0x22);
	Camera_WriteRegister(0x5000 ,0xcf);
	Camera_WriteRegister(0x5020 ,0x04);
	Camera_WriteRegister(0x5181 ,0x79);
	Camera_WriteRegister(0x5182 ,0x00);
	Camera_WriteRegister(0x5185 ,0x22);
	Camera_WriteRegister(0x5197 ,0x01);
	Camera_WriteRegister(0x5500 ,0x0a);
	Camera_WriteRegister(0x5504 ,0x00);
	Camera_WriteRegister(0x5505 ,0x7f);
	Camera_WriteRegister(0x5080 ,0x08);
	Camera_WriteRegister(0x300e ,0x18);
	Camera_WriteRegister(0x4610 ,0x00);
	Camera_WriteRegister(0x471d ,0x05);
	Camera_WriteRegister(0x4708 ,0x06);
	Camera_WriteRegister(0x3710 ,0x10);
	Camera_WriteRegister(0x370d ,0x06);
	Camera_WriteRegister(0x3632 ,0x41);
	Camera_WriteRegister(0x3702 ,0x40);
	Camera_WriteRegister(0x3620 ,0x37);
	Camera_WriteRegister(0x3631 ,0x01);
	Camera_WriteRegister(0x370c ,0xa0);
	Camera_WriteRegister(0x3808 ,0x0a);
	Camera_WriteRegister(0x3809 ,0x20);
	Camera_WriteRegister(0x380a ,0x07);
	Camera_WriteRegister(0x380b ,0x98);
	Camera_WriteRegister(0x380c ,0x0c);
	Camera_WriteRegister(0x380d ,0x80);
	Camera_WriteRegister(0x380e ,0x07);
	Camera_WriteRegister(0x380f ,0xd0);
	Camera_WriteRegister(0x5000 ,0x06);
	Camera_WriteRegister(0x501f ,0x03);
	Camera_WriteRegister(0x3503 ,0x07);
	Camera_WriteRegister(0x3501 ,0x73);
	Camera_WriteRegister(0x3502 ,0x80);
	Camera_WriteRegister(0x350b ,0x00);
	Camera_WriteRegister(0x3818 ,0xc0);
	Camera_WriteRegister(0x3621 ,0x27);
	Camera_WriteRegister(0x3801 ,0x8a);
	Camera_WriteRegister(0x3a00 ,0x78);
	Camera_WriteRegister(0x3a1a ,0x04);
	Camera_WriteRegister(0x3a13 ,0x30);
	Camera_WriteRegister(0x3a18 ,0x00);
	Camera_WriteRegister(0x3a19 ,0x7c);
	Camera_WriteRegister(0x3a08 ,0x12);
	Camera_WriteRegister(0x3a09 ,0xc0);
	Camera_WriteRegister(0x3a0a ,0x0f);
	Camera_WriteRegister(0x3a0b ,0xa0);
	Camera_WriteRegister(0x3004 ,0xff);
	Camera_WriteRegister(0x350c ,0x07);
	Camera_WriteRegister(0x350d ,0xd0);
	Camera_WriteRegister(0x3a0d ,0x08);
	Camera_WriteRegister(0x3a0e ,0x06);
	Camera_WriteRegister(0x3500 ,0x00);
	Camera_WriteRegister(0x3501 ,0x00);
	Camera_WriteRegister(0x3502 ,0x00);
	Camera_WriteRegister(0x350a ,0x00);
	Camera_WriteRegister(0x350b ,0x00);
	Camera_WriteRegister(0x3503 ,0x00);
	Camera_WriteRegister(0x3030 ,0x2b);
	Camera_WriteRegister(0x3a02 ,0x00);
	Camera_WriteRegister(0x3a03 ,0x7d);
	Camera_WriteRegister(0x3a04 ,0x00);
	Camera_WriteRegister(0x3a14 ,0x00);
	Camera_WriteRegister(0x3a15 ,0x7d);
	Camera_WriteRegister(0x3a16 ,0x00);
	Camera_WriteRegister(0x3a00 ,0x78);
	Camera_WriteRegister(0x3a08 ,0x09);
	Camera_WriteRegister(0x3a09 ,0x60);
	Camera_WriteRegister(0x3a0a ,0x07);
	Camera_WriteRegister(0x3a0b ,0xd0);
	Camera_WriteRegister(0x3a0d ,0x10);
	Camera_WriteRegister(0x3a0e ,0x0d);
	Camera_WriteRegister(0x3620 ,0x57);
	Camera_WriteRegister(0x3703 ,0x98);
	Camera_WriteRegister(0x3704 ,0x1c);
	Camera_WriteRegister(0x589b ,0x00);
	Camera_WriteRegister(0x589a ,0xc0);
	Camera_WriteRegister(0x3633 ,0x07);
	Camera_WriteRegister(0x3702 ,0x10);
	Camera_WriteRegister(0x3703 ,0xb2);
	Camera_WriteRegister(0x3704 ,0x18);
	Camera_WriteRegister(0x370b ,0x40);
	Camera_WriteRegister(0x370d ,0x02);
	Camera_WriteRegister(0x3620 ,0x52);
	Camera_WriteRegister(0x5000 ,0x06);
	Camera_WriteRegister(0x5001 ,0x00);
	Camera_WriteRegister(0x5005 ,0x00);
	
	Camera_WriteRegister(0x3818 ,0x80);
	Camera_WriteRegister(0x3621 ,0x17);
	Camera_WriteRegister(0x3801 ,0xb4);
	Camera_WriteRegister(0x3001 ,0x40);
	Camera_WriteRegister(0x3002 ,0x1c);
	Camera_WriteRegister(0x3810 ,0x00);
	Camera_WriteRegister(0x3818 ,0x00);
	Camera_WriteRegister(0x460c ,0x20);
	Camera_WriteRegister(0x501f ,0x04);
	//Camera_WriteRegister(0x4300 ,0xf8);
}

void OC5642_Processor_RAW_Setting( void )
{
	Camera_WriteRegister(0x3103 ,0x93);//PCLK CLOCK SELECT
	
	Camera_WriteRegister(0x3008 ,0x82);
	Camera_WriteRegister(0x3017 ,0x7f);
	Camera_WriteRegister(0x3018 ,0xfc);
	Camera_WriteRegister(0x3810 ,0xc2);
	Camera_WriteRegister(0x3615 ,0xf0);
	
	Camera_WriteRegister(0x3000 ,0x00);
	Camera_WriteRegister(0x3001 ,0x00);
	Camera_WriteRegister(0x3002 ,0x00);
	Camera_WriteRegister(0x3003 ,0x00);
	
	Camera_WriteRegister(0x3011 ,0x08);
	Camera_WriteRegister(0x3010 ,0x30);//PLL DIVS DIVIDER 
	Camera_WriteRegister(0x3604 ,0x60);//ANALOG CONTROL REGISTER
	Camera_WriteRegister(0x3622 ,0x08);//ANALOG CONTROL REGISTER
	Camera_WriteRegister(0x3621 ,0x17);//ARRY CONTROL
	Camera_WriteRegister(0x3709 ,0x00);//ANALOG CONTROL REGISTER
	Camera_WriteRegister(0x4000 ,0x21);
	Camera_WriteRegister(0x401d ,0x02);
	Camera_WriteRegister(0x3600 ,0x54);//ANALOG CONTROL REGISTER
	Camera_WriteRegister(0x3605 ,0x04);//ANALOG CONTROL REGISTER
	Camera_WriteRegister(0x3606 ,0x3f);//ANALOG CONTROL REGISTER
	Camera_WriteRegister(0x3c01 ,0x80);
	Camera_WriteRegister(0x300d ,0x21);
	Camera_WriteRegister(0x3623 ,0x22);//ANALOG CONTROL REGISTER
	
	Camera_WriteRegister(0x5000 ,0xcf);
	Camera_WriteRegister(0x5020 ,0x04);//ISP RESERVED


	//LENS
	{
	
	Camera_WriteRegister(0x5800 ,0x48);
	Camera_WriteRegister(0x5801 ,0x31);
	Camera_WriteRegister(0x5802 ,0x21);
	Camera_WriteRegister(0x5803 ,0x1b);
	Camera_WriteRegister(0x5804 ,0x1a);
	Camera_WriteRegister(0x5805 ,0x1e);
	Camera_WriteRegister(0x5806 ,0x29);
	Camera_WriteRegister(0x5807 ,0x38);
	Camera_WriteRegister(0x5808 ,0x26);
	Camera_WriteRegister(0x5809 ,0x17);
	Camera_WriteRegister(0x580a ,0x11);
	Camera_WriteRegister(0x580b ,0xe );
	Camera_WriteRegister(0x580c ,0xd );
	Camera_WriteRegister(0x580d ,0xe );
	Camera_WriteRegister(0x580e ,0x13);
	Camera_WriteRegister(0x580f ,0x1a);
	Camera_WriteRegister(0x5810 ,0x15);
	Camera_WriteRegister(0x5811 ,0xd );
	Camera_WriteRegister(0x5812 ,0x8 );
	Camera_WriteRegister(0x5813 ,0x5 );
	Camera_WriteRegister(0x5814 ,0x4 );
	Camera_WriteRegister(0x5815 ,0x5 );
	Camera_WriteRegister(0x5816 ,0x9 );
	Camera_WriteRegister(0x5817 ,0xd );
	Camera_WriteRegister(0x5818 ,0x11);
	Camera_WriteRegister(0x5819 ,0xa );
	Camera_WriteRegister(0x581a ,0x4 );
	Camera_WriteRegister(0x581b ,0x0 );
	Camera_WriteRegister(0x581c ,0x0 );
	Camera_WriteRegister(0x581d ,0x1 );
	Camera_WriteRegister(0x581e ,0x6 );
	Camera_WriteRegister(0x581f ,0x9 );
	Camera_WriteRegister(0x5820 ,0x12);
	Camera_WriteRegister(0x5821 ,0xb );
	Camera_WriteRegister(0x5822 ,0x4 );
	Camera_WriteRegister(0x5823 ,0x0 );
	Camera_WriteRegister(0x5824 ,0x0 );
	Camera_WriteRegister(0x5825 ,0x1 );
	Camera_WriteRegister(0x5826 ,0x6 );
	Camera_WriteRegister(0x5827 ,0xa );
	Camera_WriteRegister(0x5828 ,0x17);
	Camera_WriteRegister(0x5829 ,0xf );
	Camera_WriteRegister(0x582a ,0x9 );
	Camera_WriteRegister(0x582b ,0x6 );
	Camera_WriteRegister(0x582c ,0x5 );
	Camera_WriteRegister(0x582d ,0x6 );
	Camera_WriteRegister(0x582e ,0xa );
	Camera_WriteRegister(0x582f ,0xe );
	Camera_WriteRegister(0x5830 ,0x28);
	Camera_WriteRegister(0x5831 ,0x1a);
	Camera_WriteRegister(0x5832 ,0x11);
	Camera_WriteRegister(0x5833 ,0xe );
	Camera_WriteRegister(0x5834 ,0xe );
	Camera_WriteRegister(0x5835 ,0xf );
	Camera_WriteRegister(0x5836 ,0x15);
	Camera_WriteRegister(0x5837 ,0x1d);
	Camera_WriteRegister(0x5838 ,0x6e);
	Camera_WriteRegister(0x5839 ,0x39);
	Camera_WriteRegister(0x583a ,0x27);
	Camera_WriteRegister(0x583b ,0x1f);
	Camera_WriteRegister(0x583c ,0x1e);
	Camera_WriteRegister(0x583d ,0x23);
	Camera_WriteRegister(0x583e ,0x2f);
	Camera_WriteRegister(0x583f ,0x41);
	Camera_WriteRegister(0x5840 ,0xe );
	Camera_WriteRegister(0x5841 ,0xc );
	Camera_WriteRegister(0x5842 ,0xd );
	Camera_WriteRegister(0x5843 ,0xc );
	Camera_WriteRegister(0x5844 ,0xc );
	Camera_WriteRegister(0x5845 ,0xc );
	Camera_WriteRegister(0x5846 ,0xc );
	Camera_WriteRegister(0x5847 ,0xc );
	Camera_WriteRegister(0x5848 ,0xd );
	Camera_WriteRegister(0x5849 ,0xe );
	Camera_WriteRegister(0x584a ,0xe );
	Camera_WriteRegister(0x584b ,0xa );
	Camera_WriteRegister(0x584c ,0xe );
	Camera_WriteRegister(0x584d ,0xe );
	Camera_WriteRegister(0x584e ,0x10);
	Camera_WriteRegister(0x584f ,0x10);
	Camera_WriteRegister(0x5850 ,0x11);
	Camera_WriteRegister(0x5851 ,0xa );
	Camera_WriteRegister(0x5852 ,0xf );
	Camera_WriteRegister(0x5853 ,0xe );
	Camera_WriteRegister(0x5854 ,0x10);
	Camera_WriteRegister(0x5855 ,0x10);
	Camera_WriteRegister(0x5856 ,0x10);
	Camera_WriteRegister(0x5857 ,0xa );
	Camera_WriteRegister(0x5858 ,0xe );
	Camera_WriteRegister(0x5859 ,0xe );
	Camera_WriteRegister(0x585a ,0xf );
	Camera_WriteRegister(0x585b ,0xf );
	Camera_WriteRegister(0x585c ,0xf );
	Camera_WriteRegister(0x585d ,0xa );
	Camera_WriteRegister(0x585e ,0x9 );
	Camera_WriteRegister(0x585f ,0xd );
	Camera_WriteRegister(0x5860 ,0xc );
	Camera_WriteRegister(0x5861 ,0xb );
	Camera_WriteRegister(0x5862 ,0xd );
	Camera_WriteRegister(0x5863 ,0x7 );
	Camera_WriteRegister(0x5864 ,0x17);
	Camera_WriteRegister(0x5865 ,0x14);
	Camera_WriteRegister(0x5866 ,0x18);
	Camera_WriteRegister(0x5867 ,0x18);
	Camera_WriteRegister(0x5868 ,0x16);
	Camera_WriteRegister(0x5869 ,0x12);
	Camera_WriteRegister(0x586a ,0x1b);
	Camera_WriteRegister(0x586b ,0x1a);
	Camera_WriteRegister(0x586c ,0x16);
	Camera_WriteRegister(0x586d ,0x16);
	Camera_WriteRegister(0x586e ,0x18);
	Camera_WriteRegister(0x586f ,0x1f);
	Camera_WriteRegister(0x5870 ,0x1c);
	Camera_WriteRegister(0x5871 ,0x16);
	Camera_WriteRegister(0x5872 ,0x10);
	Camera_WriteRegister(0x5873 ,0xf );
	Camera_WriteRegister(0x5874 ,0x13);
	Camera_WriteRegister(0x5875 ,0x1c);
	Camera_WriteRegister(0x5876 ,0x1e);
	Camera_WriteRegister(0x5877 ,0x17);
	Camera_WriteRegister(0x5878 ,0x11);
	Camera_WriteRegister(0x5879 ,0x11);
	Camera_WriteRegister(0x587a ,0x14);
	Camera_WriteRegister(0x587b ,0x1e);
	Camera_WriteRegister(0x587c ,0x1c);
	Camera_WriteRegister(0x587d ,0x1c);
	Camera_WriteRegister(0x587e ,0x1a);
	Camera_WriteRegister(0x587f ,0x1a);
	Camera_WriteRegister(0x5880 ,0x1b);
	Camera_WriteRegister(0x5881 ,0x1f);
	Camera_WriteRegister(0x5882 ,0x14);
	Camera_WriteRegister(0x5883 ,0x1a);
	Camera_WriteRegister(0x5884 ,0x1d);
	Camera_WriteRegister(0x5885 ,0x1e);
	Camera_WriteRegister(0x5886 ,0x1a);
	Camera_WriteRegister(0x5887 ,0x1a);
	
	}
	//AWB
	{
	Camera_WriteRegister(0x5180 ,0xff);
	Camera_WriteRegister(0x5181 ,0x52);
	Camera_WriteRegister(0x5182 ,0x11);
	Camera_WriteRegister(0x5183 ,0x14);
	Camera_WriteRegister(0x5184 ,0x25);
	Camera_WriteRegister(0x5185 ,0x24);
	Camera_WriteRegister(0x5186 ,0x14);
	Camera_WriteRegister(0x5187 ,0x14);
	Camera_WriteRegister(0x5188 ,0x14);
	Camera_WriteRegister(0x5189 ,0x69);
	Camera_WriteRegister(0x518a ,0x60);
	Camera_WriteRegister(0x518b ,0xa2);
	Camera_WriteRegister(0x518c ,0x9c);
	Camera_WriteRegister(0x518d ,0x36);
	Camera_WriteRegister(0x518e ,0x34);
	Camera_WriteRegister(0x518f ,0x54);
	Camera_WriteRegister(0x5190 ,0x4c);
	Camera_WriteRegister(0x5191 ,0xf8);
	Camera_WriteRegister(0x5192 ,0x04);
	Camera_WriteRegister(0x5193 ,0x70);
	Camera_WriteRegister(0x5194 ,0xf0);
	Camera_WriteRegister(0x5195 ,0xf0);
	Camera_WriteRegister(0x5196 ,0x03);
	Camera_WriteRegister(0x5197 ,0x01);
	Camera_WriteRegister(0x5198 ,0x05);
	Camera_WriteRegister(0x5199 ,0x2f);
	Camera_WriteRegister(0x519a ,0x04);
	Camera_WriteRegister(0x519b ,0x00);
	Camera_WriteRegister(0x519c ,0x06);
	Camera_WriteRegister(0x519d ,0xa0);
	Camera_WriteRegister(0x519e ,0xa0);
	}
	//D/S
	{
	/*
	Camera_WriteRegister(0x528a ,0x00);
	Camera_WriteRegister(0x528b ,0x01);
	Camera_WriteRegister(0x528c ,0x04);
	Camera_WriteRegister(0x528d ,0x08);
	Camera_WriteRegister(0x528e ,0x10);
	Camera_WriteRegister(0x528f ,0x20);
	Camera_WriteRegister(0x5290 ,0x30);
	Camera_WriteRegister(0x5292 ,0x00);
	Camera_WriteRegister(0x5293 ,0x00);
	Camera_WriteRegister(0x5294 ,0x00);
	Camera_WriteRegister(0x5295 ,0x01);
	Camera_WriteRegister(0x5296 ,0x00);
	Camera_WriteRegister(0x5297 ,0x04);
	Camera_WriteRegister(0x5298 ,0x00);
	Camera_WriteRegister(0x5299 ,0x08);
	Camera_WriteRegister(0x529a ,0x00);
	Camera_WriteRegister(0x529b ,0x10);
	Camera_WriteRegister(0x529c ,0x00);
	Camera_WriteRegister(0x529d ,0x20);
	Camera_WriteRegister(0x529e ,0x00);
	Camera_WriteRegister(0x529f ,0x30);
	Camera_WriteRegister(0x5282 ,0x00);
	Camera_WriteRegister(0x5300 ,0x00);
	Camera_WriteRegister(0x5301 ,0x20);
	Camera_WriteRegister(0x5302 ,0x00);
	Camera_WriteRegister(0x5303 ,0x7c);
	Camera_WriteRegister(0x530c ,0x00);
	Camera_WriteRegister(0x530d ,0x10);
	Camera_WriteRegister(0x530e ,0x20);
	Camera_WriteRegister(0x530f ,0x80);
	Camera_WriteRegister(0x5310 ,0x20);
	Camera_WriteRegister(0x5311 ,0x80);
	Camera_WriteRegister(0x5308 ,0x20);
	Camera_WriteRegister(0x5309 ,0x40);
	Camera_WriteRegister(0x5304 ,0x00);
	Camera_WriteRegister(0x5305 ,0x30);
	Camera_WriteRegister(0x5306 ,0x00);
	Camera_WriteRegister(0x5307 ,0x80);
	Camera_WriteRegister(0x5314 ,0x08);
	Camera_WriteRegister(0x5315 ,0x20);
	Camera_WriteRegister(0x5319 ,0x30);
	Camera_WriteRegister(0x5316 ,0x10);
	Camera_WriteRegister(0x5317 ,0x00);
	Camera_WriteRegister(0x5318 ,0x02);
	*/
	}
	//CMX
	{

	Camera_WriteRegister(0x5380 ,0x01);
	Camera_WriteRegister(0x5381 ,0x00);
	Camera_WriteRegister(0x5382 ,0x00);
	Camera_WriteRegister(0x5383 ,0x1f);
	Camera_WriteRegister(0x5384 ,0x00);
	Camera_WriteRegister(0x5385 ,0x06);
	Camera_WriteRegister(0x5386 ,0x00);
	Camera_WriteRegister(0x5387 ,0x00);
	Camera_WriteRegister(0x5388 ,0x00);
	Camera_WriteRegister(0x5389 ,0xE1);
	Camera_WriteRegister(0x538A ,0x00);
	Camera_WriteRegister(0x538B ,0x2B);
	Camera_WriteRegister(0x538C ,0x00);
	Camera_WriteRegister(0x538D ,0x00);
	Camera_WriteRegister(0x538E ,0x00);
	Camera_WriteRegister(0x538F ,0x10);
	Camera_WriteRegister(0x5390 ,0x00);
	Camera_WriteRegister(0x5391 ,0xB3);
	Camera_WriteRegister(0x5392 ,0x00);
	Camera_WriteRegister(0x5393 ,0xA6);
	Camera_WriteRegister(0x5394 ,0x08);	

	}
	//GAMMA
	{
	Camera_WriteRegister(0x5480 ,0x0c);
	Camera_WriteRegister(0x5481 ,0x18);
	Camera_WriteRegister(0x5482 ,0x2f);
	Camera_WriteRegister(0x5483 ,0x55);
	Camera_WriteRegister(0x5484 ,0x64);
	Camera_WriteRegister(0x5485 ,0x71);
	Camera_WriteRegister(0x5486 ,0x7d);
	Camera_WriteRegister(0x5487 ,0x87);
	Camera_WriteRegister(0x5488 ,0x91);
	Camera_WriteRegister(0x5489 ,0x9a);
	Camera_WriteRegister(0x548A ,0xaa);
	Camera_WriteRegister(0x548B ,0xb8);
	Camera_WriteRegister(0x548C ,0xcd);
	Camera_WriteRegister(0x548D ,0xdd);
	Camera_WriteRegister(0x548E ,0xea);
	Camera_WriteRegister(0x548F ,0x1d);
	Camera_WriteRegister(0x5490 ,0x05);
	Camera_WriteRegister(0x5491 ,0x00);
	Camera_WriteRegister(0x5492 ,0x04);
	Camera_WriteRegister(0x5493 ,0x20);
	Camera_WriteRegister(0x5494 ,0x03);
	Camera_WriteRegister(0x5495 ,0x60);
	Camera_WriteRegister(0x5496 ,0x02);
	Camera_WriteRegister(0x5497 ,0xB8);
	Camera_WriteRegister(0x5498 ,0x02);
	Camera_WriteRegister(0x5499 ,0x86);
	Camera_WriteRegister(0x549A ,0x02);
	Camera_WriteRegister(0x549B ,0x5B);
	Camera_WriteRegister(0x549C ,0x02);
	Camera_WriteRegister(0x549D ,0x3B);
	Camera_WriteRegister(0x549E ,0x02);
	Camera_WriteRegister(0x549F ,0x1C);
	Camera_WriteRegister(0x54A0 ,0x02);
	Camera_WriteRegister(0x54A1 ,0x04);
	Camera_WriteRegister(0x54A2 ,0x01);
	Camera_WriteRegister(0x54A3 ,0xED);
	Camera_WriteRegister(0x54A4 ,0x01);
	Camera_WriteRegister(0x54A5 ,0xC5);
	Camera_WriteRegister(0x54A6 ,0x01);
	Camera_WriteRegister(0x54A7 ,0xA5);
	Camera_WriteRegister(0x54A8 ,0x01);
	Camera_WriteRegister(0x54A9 ,0x6C);
	Camera_WriteRegister(0x54AA ,0x01);
	Camera_WriteRegister(0x54AB ,0x41);
	Camera_WriteRegister(0x54AC ,0x01);
	Camera_WriteRegister(0x54AD ,0x20);
	Camera_WriteRegister(0x54AE ,0x00);
	Camera_WriteRegister(0x54AF ,0x16);
	Camera_WriteRegister(0x54B0 ,0x01);
	Camera_WriteRegister(0x54B1 ,0x20);
	Camera_WriteRegister(0x54B2 ,0x00);
	Camera_WriteRegister(0x54B3 ,0x10);
	Camera_WriteRegister(0x54B4 ,0x00);
	Camera_WriteRegister(0x54B5 ,0xf0);
	Camera_WriteRegister(0x54B6 ,0x00);
	Camera_WriteRegister(0x54B7 ,0xDF);
	Camera_WriteRegister(0x5402 ,0x3f);
	Camera_WriteRegister(0x5403 ,0x00);
	}
	//UV ADJUST
	{
	Camera_WriteRegister(0x5500 ,0x10);
	Camera_WriteRegister(0x5502 ,0x00);
	Camera_WriteRegister(0x5503 ,0x06);
	Camera_WriteRegister(0x5504 ,0x00);
	Camera_WriteRegister(0x5505 ,0x7f);
	}
	//AE
	{
		
	Camera_WriteRegister(0x5025 ,0x80);
	Camera_WriteRegister(0x3a0f ,0x30);
	Camera_WriteRegister(0x3a10 ,0x28);
	Camera_WriteRegister(0x3a1b ,0x30);
	Camera_WriteRegister(0x3a1e ,0x28);
	Camera_WriteRegister(0x3a11 ,0x61);
	Camera_WriteRegister(0x3a1f ,0x10);
	Camera_WriteRegister(0x5688 ,0xfd);
	Camera_WriteRegister(0x5689 ,0xdf);
	Camera_WriteRegister(0x568a ,0xfe);
	Camera_WriteRegister(0x568b ,0xef);
	Camera_WriteRegister(0x568c ,0xfe);
	Camera_WriteRegister(0x568d ,0xef);
	Camera_WriteRegister(0x568e ,0xaa);
	Camera_WriteRegister(0x568f ,0xaa);
		
	}
	
	
	
	//Camera_WriteRegister(0x5500 ,0x0a);//UV
	//Camera_WriteRegister(0x5504 ,0x00);
	//Camera_WriteRegister(0x5505 ,0x7f);
	
	Camera_WriteRegister(0x5080 ,0x08);//EVEN CTRL(THRESHOLD)
	
	Camera_WriteRegister(0x300e ,0x18);//MIPI POWER DOWN
	
	Camera_WriteRegister(0x4610 ,0x00);
	
	Camera_WriteRegister(0x471d ,0x05);//DVP CONTROL
	Camera_WriteRegister(0x4708 ,0x06);
	
	Camera_WriteRegister(0x3710 ,0x10);
	Camera_WriteRegister(0x370d ,0x06);//VERTICAL BINNING DISABLE
	
	Camera_WriteRegister(0x3632 ,0x41);//ANALOG CONTROL REGISTER
	Camera_WriteRegister(0x3702 ,0x40);//ANALOG CONTROL REGISTER
	Camera_WriteRegister(0x3620 ,0x37);//ANALOG CONTROL REGISTER
	Camera_WriteRegister(0x3631 ,0x01);//ANALOG CONTROL REGISTER
	Camera_WriteRegister(0x370c ,0xa0);//ANALOG CONTROL REGISTER
	
	
	Camera_WriteRegister(0x3808 ,0x0a);//2952
	Camera_WriteRegister(0x3809 ,0x20);
	Camera_WriteRegister(0x380a ,0x07);//1944
	Camera_WriteRegister(0x380b ,0x98);
	
	
	//1024 x 768
	/*
	Camera_WriteRegister(0x3808,0x04);//TIMING DVP OUTPUT HORIZONTAL WIDTH HIGH 
	Camera_WriteRegister(0x3809,0x00);//TIMING DVP OUTPUT HORIZONTAL WIDTH LOW
	Camera_WriteRegister(0x380a,0x03);//TIMING DVP OUTPUT VERTICAL HEIGHT HIGH
	Camera_WriteRegister(0x380b,0x00);//TIMING DVP OUTPUT VERTICAL HEIGHT LOW	
	*/

	Camera_WriteRegister(0x380c ,0x0c);//3200
	Camera_WriteRegister(0x380d ,0x80);
	Camera_WriteRegister(0x380e ,0x07);//2000
	Camera_WriteRegister(0x380f ,0xd0);
	
	/*
	Camera_WriteRegister(0x3503 ,0x07);//AEC( 자동노출 )
	Camera_WriteRegister(0x3501 ,0x73);
	Camera_WriteRegister(0x3502 ,0x80);
	Camera_WriteRegister(0x350b ,0x00);
	*/
	//Camera_WriteRegister(0x3818 ,0xc0);
	//Camera_WriteRegister(0x3621 ,0x27);
	//Camera_WriteRegister(0x3801 ,0x8a);//HREF HORIZONTAL START POINT LOW
	
	
	Camera_WriteRegister(0x3a00 ,0x78);//AEC/AGE
	Camera_WriteRegister(0x3a1a ,0x04);
	Camera_WriteRegister(0x3a13 ,0x30);
	Camera_WriteRegister(0x3a18 ,0x00);
	Camera_WriteRegister(0x3a19 ,0x7c);
	Camera_WriteRegister(0x3a08 ,0x12);
	Camera_WriteRegister(0x3a09 ,0xc0);
	Camera_WriteRegister(0x3a0a ,0x0f);
	Camera_WriteRegister(0x3a0b ,0xa0);
	
	Camera_WriteRegister(0x350c ,0x07);//AEC//AGE
	Camera_WriteRegister(0x350d ,0xd0);
	Camera_WriteRegister(0x3a0d ,0x08);
	Camera_WriteRegister(0x3a0e ,0x06);
	



	Camera_WriteRegister(0x3004 ,0xff);//CLOCK ENABLE ALL
	
		
	Camera_WriteRegister(0x3500 ,0x00);
	Camera_WriteRegister(0x3501 ,0x00);
	Camera_WriteRegister(0x3502 ,0x00);
	Camera_WriteRegister(0x350a ,0x00);
	Camera_WriteRegister(0x350b ,0x00);
	Camera_WriteRegister(0x3503 ,0x00);
	
	Camera_WriteRegister(0x3030 ,0x2b);//SYSTEM IO BP_REGULATOR
	
	Camera_WriteRegister(0x3a02 ,0x00);
	Camera_WriteRegister(0x3a03 ,0x7d);
	Camera_WriteRegister(0x3a04 ,0x00);
	Camera_WriteRegister(0x3a14 ,0x00);
	Camera_WriteRegister(0x3a15 ,0x7d);
	Camera_WriteRegister(0x3a16 ,0x00);
	Camera_WriteRegister(0x3a00 ,0x78);
	Camera_WriteRegister(0x3a08 ,0x09);
	Camera_WriteRegister(0x3a09 ,0x60);
	Camera_WriteRegister(0x3a0a ,0x07);
	Camera_WriteRegister(0x3a0b ,0xd0);
	Camera_WriteRegister(0x3a0d ,0x10);
	Camera_WriteRegister(0x3a0e ,0x0d);
	
	Camera_WriteRegister(0x3620 ,0x57);//ANALOG CONTROL REGISTER
	Camera_WriteRegister(0x3703 ,0x98);
	Camera_WriteRegister(0x3704 ,0x1c);
	
	Camera_WriteRegister(0x589b ,0x00);// ?
	Camera_WriteRegister(0x589a ,0xc0);
	
	Camera_WriteRegister(0x3633 ,0x07);//ANALOG CONTROL REGISTER
	Camera_WriteRegister(0x3702 ,0x10);
	Camera_WriteRegister(0x3703 ,0xb2);
	Camera_WriteRegister(0x3704 ,0x18);
	Camera_WriteRegister(0x370b ,0x40);
	Camera_WriteRegister(0x370d ,0x02);
	Camera_WriteRegister(0x3620 ,0x52);
	
	
	Camera_WriteRegister(0x5000 ,0xCF);
	Camera_WriteRegister(0x5001 ,0xCF);
	Camera_WriteRegister(0x5002 ,0x60);
	Camera_WriteRegister(0x501f ,0x00);//FORMAT MUX CONTROL 
	
	Camera_WriteRegister(0x3818 ,0xA0);
	Camera_WriteRegister(0x3621 ,0x17);//ANALOG CONTROL REGISTER
	Camera_WriteRegister(0x3801 ,0xb4);//HREF HORIZONTAL START POINT LOW
	
	//영상
	Camera_WriteRegister(0x4300 ,0x32);//YUV 422, UYVY 설정
	Camera_WriteRegister(0x4730,0x01);//CCIR656 MODE ENABLE
	
}


#define OV5642_WIDTH		                    1024
#define OV5642_HEIGHT		               	768
#define OV5642_TOTAL_WIDTH	            3200
#define OV5642_TOTAL_HEIGHT	            2000
#define OV5642_SENSOR_SIZE_X	        2592
#define OV5642_SENSOR_SIZE_Y	            1944

void OV5642_Setting1(void)
{
//	uint16_t data;
	
	Camera_WriteRegister(0x3103,0x93);//PCLK CLOCK SELECT 
	Camera_WriteRegister(0x3008,0x82);//RESET SOFTWARE
	//Camera_WriteRegister(0x3008,0x02);
	
	Camera_WriteRegister(0x3017,0x7f);//PAD OUTPUT ENABLE 01
	Camera_WriteRegister(0x3018,0xfc);//PAD OUTPUT ENABLE 02
	
	Camera_WriteRegister(0x3810,0xc2);//PLL CONTROL
	Camera_WriteRegister(0x3615,0xf0);//ANALOG CONTROL REGISTER
	
	Camera_WriteRegister(0x3000,0x00);//SYSTEM RESET 00
	Camera_WriteRegister(0x3001,0x00);//SYSTEM RESET 01
	Camera_WriteRegister(0x3002,0x00);//SYSTEM RESET 02
	Camera_WriteRegister(0x3003,0x00);//SYSTEM RESET 03
	
	Camera_WriteRegister(0x3000,0xf8);
	Camera_WriteRegister(0x3001,0x48);
	Camera_WriteRegister(0x3002,0x5c);
	Camera_WriteRegister(0x3003,0x02);
	Camera_WriteRegister(0x3004,0x07);
	Camera_WriteRegister(0x3005,0xff);
	Camera_WriteRegister(0x3006,0xff);
	Camera_WriteRegister(0x3007,0x07);
	Camera_WriteRegister(0x3011,0x08);
	
	Camera_WriteRegister(0x3010,0x10);//PLL CONTROL
	
	Camera_WriteRegister(0x460c,0x22);//PCLK 0X20 AUTOMODE  0X22 PCLK MANUAL MODE
	Camera_WriteRegister(0x3815,0x01);//PCLK MANUAL SET
	
			Camera_WriteRegister(0x370d,0x06);//ANALOG CONTROL REGISTER
			Camera_WriteRegister(0x370c,0xa0);//ANALOG CONTROL REGISTER
	
			Camera_WriteRegister(0x3602,0xfc);//ANALOG CONTROL REGISTER
			Camera_WriteRegister(0x3612,0xff);//ANALOG CONTROL REGISTER
			Camera_WriteRegister(0x3634,0xc0);//ANALOG CONTROL REGISTER
			Camera_WriteRegister(0x3613,0x00);//ANALOG CONTROL REGISTER
			Camera_WriteRegister(0x3605,0x7c);//ANALOG CONTROL REGISTER
			Camera_WriteRegister(0x3621,0x09);//ANALOG CONTROL REGISTER
			Camera_WriteRegister(0x3622,0x00);//ANALOG CONTROL REGISTER
			Camera_WriteRegister(0x3604,0x40);//ANALOG CONTROL REGISTER
			Camera_WriteRegister(0x3603,0xa7); //ANALOG CONTROL REGISTER
			Camera_WriteRegister(0x3603,0x27);//ANALOG CONTROL REGISTER
			
	Camera_WriteRegister(0x4000,0x21);//BLACK LEVEL CALIBRATION [BLC] SIGNAL ENABLE
	Camera_WriteRegister(0x401d,0x02);//BLC FRAME CTRL

			Camera_WriteRegister(0x3600,0x54);//ANALOG CONTROL REGISTER
			Camera_WriteRegister(0x3605,0x04);//ANALOG CONTROL REGISTER
			Camera_WriteRegister(0x3606,0x3f);//ANALOG CONTROL REGISTER
			
	Camera_WriteRegister(0x3c01,0x80);//LIGHT FREQUENCY REGISTER >> =AUTO DETECTION ENABLE
	
	Camera_WriteRegister(0x5000,0x4f);//ISP CONTROL 00
	Camera_WriteRegister(0x5020,0x04);//ISP RESERVED
	
	Camera_WriteRegister(0x5181,0x79);//AWB
	Camera_WriteRegister(0x5182,0x00);//AWB
	Camera_WriteRegister(0x5185,0x22);//AWB
	Camera_WriteRegister(0x5197,0x01);//AWB
	
	//Camera_WriteRegister(0x5001,0xff);//ISP CONTROL 01
	Camera_WriteRegister(0x5001,0xCF);//ISP CONTROL 01

	Camera_WriteRegister(0x5500,0x0a);//UV CONTROL
	Camera_WriteRegister(0x5504,0x00);//UV
	Camera_WriteRegister(0x5505,0x7f);//UV
	
	Camera_WriteRegister(0x5080,0x08);//ISP RESERVED
	
	Camera_WriteRegister(0x300e,0x18);//ISP MIPI CONTROL POWER DOWN RX,TX
	
	Camera_WriteRegister(0x4610,0x00);//NONE
	
	Camera_WriteRegister(0x471d,0x05);//DVP CONTROL
	Camera_WriteRegister(0x4708,0x06);//DVP
	
		Camera_WriteRegister(0x3710,0x10);//ANALOG CONTROL REGISTER
		Camera_WriteRegister(0x3632,0x41);//ANALOG CONTROL REGISTER
		Camera_WriteRegister(0x3702,0x40);//ANALOG CONTROL REGISTER
		Camera_WriteRegister(0x3620,0x37);//ANALOG CONTROL REGISTER
		Camera_WriteRegister(0x3631,0x01);//ANALOG CONTROL REGISTER
	
	Camera_WriteRegister(0x3808,0x02);//TIMING  DVPHO
	Camera_WriteRegister(0x3809,0x80);//TIMING  DVPHO
	Camera_WriteRegister(0x380a,0x01);//TIMING  DVPVO
	Camera_WriteRegister(0x380b,0xe0);//TIMING  DVPVO
	Camera_WriteRegister(0x380e,0x07);//TIMING  VTS
	Camera_WriteRegister(0x380f,0xd0);//TIMING  VTS

	//Camera_WriteRegister(0x501f,0x00); //POINT FORMAT MUX CONTROL // 설정값
	Camera_WriteRegister(0x501f,0x00); //POINT FORMAT MUX CONTROL // ISP  0-YUV,1- RGB, 2-YUV,3-RAW,4-CIF RAW,5-CIF RAW,6-CIF YUV
	
	//Camera_WriteRegister(0x5000,0x4f);//ISP CONTROL 00 //중복선언
	//Camera_WriteRegister(0x4300,0x30);//FORMAT :: OUTPUT 체크 하단부에서 선언
	
	Camera_WriteRegister(0x3503,0x07);//AEC PK MANUAL. VTS,AGC,AEC MANUAL ENABLE 자동노출
	Camera_WriteRegister(0x3501,0x73);//AEC
	Camera_WriteRegister(0x3502,0x80);//AEC
	Camera_WriteRegister(0x350b,0x00);//AEC
	Camera_WriteRegister(0x3503,0x07);//AEC
	
	Camera_WriteRegister(0x3824,0x11);//NONE
	
	Camera_WriteRegister(0x3501,0x1e);//AEC
	Camera_WriteRegister(0x3502,0x80);//AEC
	Camera_WriteRegister(0x350b,0x7f);//AEC
	
	
	//3200 * 2000
	Camera_WriteRegister(0x380c,0x0C);//TIMING  HTS
	Camera_WriteRegister(0x380d,0x80);//TIMING HTS
	Camera_WriteRegister(0x380e,0x07);//TIMING VTS // 중복설정 여기가 1000으로 설정이 되어 화면이 출력이 안된것으로 보임
	Camera_WriteRegister(0x380f,0xD0);//TIMING VTS //중복설정
	
	Camera_WriteRegister(0x3a0d,0x04);//POWER DOWN DOMAIN AEC/AGC
	Camera_WriteRegister(0x3a0e,0x03);
	
	//Camera_WriteRegister(0x3818,0xc1);//TIMING CONTROL REGISTER 18 매우중요
	//Camera_WriteRegister(0x3818,0xc0);//MIRROR ON ENABLE VERTICAL FLIP
	
	Camera_WriteRegister(0x3705,0xdb);//ANALOG CONTROL
	Camera_WriteRegister(0x370a,0x81);//ANALOG CONTROL
	
	Camera_WriteRegister(0x3800,0x01);//TIMING HREF HORIZONTAL START POINT HIGH
	Camera_WriteRegister(0x3801,0x8A);//TIMING HREF HORIZONTAL START POINT LOW
	
	//Camera_WriteRegister(0x3621,0xc7);//SENSOR CONTROL ARRAY  HORIZONTAL BINING ENABLE : 매우중요
	Camera_WriteRegister(0x3621,0x10);//HORIZONTAL BINNING DISABLE
	
	Camera_WriteRegister(0x3802,0x00);//TIMING HREF VERTICAL START POINT LOW
	Camera_WriteRegister(0x3803,0x0A);//TIMING HREF VERTICAL START POINT LOW
	
	Camera_WriteRegister(0x3827,0x08);//NONE
	
	Camera_WriteRegister(0x3810,0xc0);//TIMING HORIZONTAL AND VERTICAL OFFSET SETING : 매우 중요
	
	Camera_WriteRegister(0x3804,0x06);// TIMING HREF HORIZONTAL WIDTH HIGH
	Camera_WriteRegister(0x3805,0x40);//TIMING HREF HORIZONTAL WIDTH LOW
	Camera_WriteRegister(0x5682,0x06);//SCALE HORIZONTAL END POSITION FOR AVERAGE WINDOW
	Camera_WriteRegister(0x5683,0x40);//SCALE HORIZONTAL END POSITION FOR AVERAGE WINDOW
	
	Camera_WriteRegister(0x3806,0x04);//TIMING HREF VERTICAL HEIGHT HIGH 
	Camera_WriteRegister(0x3807,0xb0);//TIMING HREF VERTICAL HEIGHT LOW
	Camera_WriteRegister(0x5686,0x04);//SCALE VERTICAL END POSITION FOR AVERAGE WINDOW
	Camera_WriteRegister(0x5687,0xb0);//SCALE VERTICAL END POSITION FOR AVERAGE WINDOW
	
	Camera_WriteRegister(0x3a00,0x78);//AEC CONTROL 00 LESS ONE LINE MODE, BAND FUNCTION, BAND LOW LIMIT MODE
	Camera_WriteRegister(0x3a1a,0x04);//AEC CONTROL REGISTER
	Camera_WriteRegister(0x3a13,0x30);//AEC CONTROL REGISTER
	Camera_WriteRegister(0x3a18,0x00);//AEC CONTROL REGISTER
	Camera_WriteRegister(0x3a19,0x7c);//AEC CONTROL REGISTER
	Camera_WriteRegister(0x3a08,0x12);//AEC CONTROL REGISTER
	Camera_WriteRegister(0x3a09,0xc0);//AEC CONTROL REGISTER
	Camera_WriteRegister(0x3a0a,0x0f);//AEC CONTROL REGISTER
	Camera_WriteRegister(0x3a0b,0xa0);//AEC CONTROL REGISTER
	
	Camera_WriteRegister(0x3004,0xff);//CLOCK ENABLE ALL ENABLE : 재설정 하는이유?
	
	Camera_WriteRegister(0x350c,0x07);//AEC VTS HIGH BITS
	Camera_WriteRegister(0x350d,0xd0);//AEC VTS LOW BITS
	Camera_WriteRegister(0x3500,0x00);//AEC EXPOSURE(노출설정)
	Camera_WriteRegister(0x3501,0x00);//AEC CONTROL REGISTER
	Camera_WriteRegister(0x3502,0x00);//AEC CONTROL REGISTER
	Camera_WriteRegister(0x350a,0x00);//AEC CONTROL REGISTER
	Camera_WriteRegister(0x350b,0x00);//AEC CONTROL REGISTER
	Camera_WriteRegister(0x3503,0x00);//AEC CONTROL REGISTER
	
	Camera_WriteRegister(0x528a,0x02);//DE-NOISE
	Camera_WriteRegister(0x528b,0x04);//DE-NOISE
	Camera_WriteRegister(0x528c,0x08);//DE-NOISE
	Camera_WriteRegister(0x528d,0x08);//DE-NOISE
	Camera_WriteRegister(0x528e,0x08);//DE-NOISE
	Camera_WriteRegister(0x528f,0x10);//DE-NOISE
	Camera_WriteRegister(0x5290,0x10);//DE-NOISE
	Camera_WriteRegister(0x5292,0x00);//DE-NOISE
	Camera_WriteRegister(0x5293,0x02);//DE-NOISE
	Camera_WriteRegister(0x5294,0x00);//DE-NOISE
	Camera_WriteRegister(0x5295,0x02);//DE-NOISE
	Camera_WriteRegister(0x5296,0x00);//DE-NOISE
	Camera_WriteRegister(0x5297,0x02);//DE-NOISE
	Camera_WriteRegister(0x5298,0x00);//DE-NOISE
	Camera_WriteRegister(0x5299,0x02);//DE-NOISE
	Camera_WriteRegister(0x529a,0x00);//DE-NOISE
	Camera_WriteRegister(0x529b,0x02);//DE-NOISE
	Camera_WriteRegister(0x529c,0x00);//DE-NOISE
	Camera_WriteRegister(0x529d,0x02);//DE-NOISE
	Camera_WriteRegister(0x529e,0x00);//DE-NOISE
	Camera_WriteRegister(0x529f,0x02);//DE-NOISE
	
	Camera_WriteRegister(0x3a0f,0x3c);//AEC/AGE
	Camera_WriteRegister(0x3a10,0x30);//AEC/AGE
	Camera_WriteRegister(0x3a1b,0x3c);//AEC/AGE
	Camera_WriteRegister(0x3a1e,0x30);//AEC/AGE
	Camera_WriteRegister(0x3a11,0x70);//AEC/AGE
	Camera_WriteRegister(0x3a1f,0x10);//AEC/AGE
	
	Camera_WriteRegister(0x3030,0x0b);//PWC CONTORL 초기값
	
	Camera_WriteRegister(0x3a02,0x00);//AEC/AGE
	Camera_WriteRegister(0x3a03,0x7d);//AEC/AGE
	Camera_WriteRegister(0x3a04,0x00);//AEC/AGE
	Camera_WriteRegister(0x3a14,0x00);//AEC/AGE
	Camera_WriteRegister(0x3a15,0x7d);//AEC/AGE
	Camera_WriteRegister(0x3a16,0x00);//AEC/AGE
	Camera_WriteRegister(0x3a00,0x7c);//AEC/AGE
	Camera_WriteRegister(0x3a08,0x09);//AEC/AGE
	Camera_WriteRegister(0x3a09,0x60);//AEC/AGE
	Camera_WriteRegister(0x3a0a,0x07);//AEC/AGE
	Camera_WriteRegister(0x3a0b,0xd0);//AEC/AGE
	Camera_WriteRegister(0x3a0d,0x08);//AEC/AGE
	Camera_WriteRegister(0x3a0e,0x06);//AEC/AGE
	
	Camera_WriteRegister(0x5193,0x70);//AWB AUTOMATIC WHITE BLANCE
	
	Camera_WriteRegister(0x3620,0x57);//ANALOG CONTROL REGISTER
	Camera_WriteRegister(0x3703,0x98);//ANALOG CONTROL REGISTER
	Camera_WriteRegister(0x3704,0x1c);//ANALOG CONTROL REGISTER
	
	Camera_WriteRegister(0x589b,0x04);//LENC
	Camera_WriteRegister(0x589a,0xc5);//LENC
	
	Camera_WriteRegister(0x528a,0x00);//DE-NOISE
	Camera_WriteRegister(0x528b,0x02);//DE-NOISE
	Camera_WriteRegister(0x528c,0x08);//DE-NOISE
	Camera_WriteRegister(0x528d,0x10);//DE-NOISE
	Camera_WriteRegister(0x528e,0x20);//DE-NOISE
	Camera_WriteRegister(0x528f,0x28);//DE-NOISE
	Camera_WriteRegister(0x5290,0x30);//DE-NOISE
	Camera_WriteRegister(0x5292,0x00);//DE-NOISE
	Camera_WriteRegister(0x5293,0x00);//DE-NOISE
	Camera_WriteRegister(0x5294,0x00);//DE-NOISE
	Camera_WriteRegister(0x5295,0x02);//DE-NOISE
	Camera_WriteRegister(0x5296,0x00);//DE-NOISE
	Camera_WriteRegister(0x5297,0x08);//DE-NOISE
	Camera_WriteRegister(0x5298,0x00);//DE-NOISE
	Camera_WriteRegister(0x5299,0x10);//DE-NOISE
	Camera_WriteRegister(0x529a,0x00);//DE-NOISE
	Camera_WriteRegister(0x529b,0x20);//DE-NOISE
	Camera_WriteRegister(0x529c,0x00);//DE-NOISE
	Camera_WriteRegister(0x529d,0x28);//DE-NOISE
	Camera_WriteRegister(0x529e,0x00);//DE-NOISE
	Camera_WriteRegister(0x529f,0x30);//DE-NOISE
	Camera_WriteRegister(0x5282,0x00);//DE-NOISE
	Camera_WriteRegister(0x5300,0x00);//DE-NOISE
	Camera_WriteRegister(0x5301,0x20);//DE-NOISE
	Camera_WriteRegister(0x5302,0x00);//DE-NOISE
	Camera_WriteRegister(0x5303,0x7c);//DE-NOISE
	Camera_WriteRegister(0x530c,0x00);//DE-NOISE
	Camera_WriteRegister(0x530d,0x0c);//DE-NOISE
	Camera_WriteRegister(0x530e,0x20);//DE-NOISE
	Camera_WriteRegister(0x530f,0x80);//DE-NOISE
	Camera_WriteRegister(0x5310,0x20);//DE-NOISE
	Camera_WriteRegister(0x5311,0x80);//DE-NOISE
	Camera_WriteRegister(0x5308,0x20);//DE-NOISE
	
	Camera_WriteRegister(0x5309,0x40);//CIP - COLOR INTERPOLATION
	Camera_WriteRegister(0x5304,0x00);//CIP - COLOR INTERPOLATION
	Camera_WriteRegister(0x5305,0x30);//CIP - COLOR INTERPOLATION
	Camera_WriteRegister(0x5306,0x00);//CIP - COLOR INTERPOLATION
	Camera_WriteRegister(0x5307,0x80);//CIP - COLOR INTERPOLATION
	Camera_WriteRegister(0x5314,0x08);//CIP - COLOR INTERPOLATION
	Camera_WriteRegister(0x5315,0x20);//CIP - COLOR INTERPOLATION
	Camera_WriteRegister(0x5319,0x30);//CIP - COLOR INTERPOLATION
	Camera_WriteRegister(0x5316,0x10);//CIP - COLOR INTERPOLATION
	Camera_WriteRegister(0x5317,0x08);//CIP - COLOR INTERPOLATION
	Camera_WriteRegister(0x5318,0x02);//CIP - COLOR INTERPOLATION
	
	Camera_WriteRegister(0x5380,0x01);//CMX COLOR MATRIX
	Camera_WriteRegister(0x5381,0x00);//CMX COLOR MATRIX
	Camera_WriteRegister(0x5382,0x00);//CMX COLOR MATRIX
	Camera_WriteRegister(0x5383,0x4e);//CMX COLOR MATRIX
	Camera_WriteRegister(0x5384,0x00);//CMX COLOR MATRIX
	Camera_WriteRegister(0x5385,0x0f);//CMX COLOR MATRIX
	Camera_WriteRegister(0x5386,0x00);//CMX COLOR MATRIX
	Camera_WriteRegister(0x5387,0x00);//CMX COLOR MATRIX
	Camera_WriteRegister(0x5388,0x01);//CMX COLOR MATRIX
	Camera_WriteRegister(0x5389,0x15);//CMX COLOR MATRIX
	Camera_WriteRegister(0x538a,0x00);//CMX COLOR MATRIX
	Camera_WriteRegister(0x538b,0x31);//CMX COLOR MATRIX
	Camera_WriteRegister(0x538c,0x00);//CMX COLOR MATRIX
	Camera_WriteRegister(0x538d,0x00);//CMX COLOR MATRIX
	Camera_WriteRegister(0x538e,0x00);//CMX COLOR MATRIX
	Camera_WriteRegister(0x538f,0x0f);//CMX COLOR MATRIX
	Camera_WriteRegister(0x5390,0x00);//CMX COLOR MATRIX
	Camera_WriteRegister(0x5391,0xab);//CMX COLOR MATRIX
	Camera_WriteRegister(0x5392,0x00);//CMX COLOR MATRIX
	Camera_WriteRegister(0x5393,0xa2);//CMX COLOR MATRIX
	Camera_WriteRegister(0x5394,0x08);//CMX COLOR MATRIX
	
	
	Camera_WriteRegister(0x5480,0x14);//GAMMA
	Camera_WriteRegister(0x5481,0x21);
	Camera_WriteRegister(0x5482,0x36);
	Camera_WriteRegister(0x5483,0x57);
	Camera_WriteRegister(0x5484,0x65);
	Camera_WriteRegister(0x5485,0x71);
	Camera_WriteRegister(0x5486,0x7d);
	Camera_WriteRegister(0x5487,0x87);
	Camera_WriteRegister(0x5488,0x91);
	Camera_WriteRegister(0x5489,0x9a);
	Camera_WriteRegister(0x548a,0xaa);
	Camera_WriteRegister(0x548b,0xb8);
	Camera_WriteRegister(0x548c,0xcd);
	Camera_WriteRegister(0x548d,0xdd);
	Camera_WriteRegister(0x548e,0xea);
	Camera_WriteRegister(0x548f,0x10);
	
	
	Camera_WriteRegister(0x5490,0x05);//UV GAMMA 
	Camera_WriteRegister(0x5491,0x00);
	Camera_WriteRegister(0x5492,0x04);
	Camera_WriteRegister(0x5493,0x20);
	Camera_WriteRegister(0x5494,0x03);
	Camera_WriteRegister(0x5495,0x60);
	Camera_WriteRegister(0x5496,0x02);
	Camera_WriteRegister(0x5497,0xb8);
	Camera_WriteRegister(0x5498,0x02);
	Camera_WriteRegister(0x5499,0x86);
	Camera_WriteRegister(0x549a,0x02);
	Camera_WriteRegister(0x549b,0x5b);
	Camera_WriteRegister(0x549c,0x02);
	Camera_WriteRegister(0x549d,0x3b);
	Camera_WriteRegister(0x549e,0x02);
	Camera_WriteRegister(0x549f,0x1c);
	Camera_WriteRegister(0x54a0,0x02);
	Camera_WriteRegister(0x54a1,0x04);
	Camera_WriteRegister(0x54a2,0x01);
	Camera_WriteRegister(0x54a3,0xed);
	Camera_WriteRegister(0x54a4,0x01);
	Camera_WriteRegister(0x54a5,0xc5);
	Camera_WriteRegister(0x54a6,0x01);
	Camera_WriteRegister(0x54a7,0xa5);
	Camera_WriteRegister(0x54a8,0x01);
	Camera_WriteRegister(0x54a9,0x6c);
	Camera_WriteRegister(0x54aa,0x01);
	Camera_WriteRegister(0x54ab,0x41);
	Camera_WriteRegister(0x54ac,0x01);
	Camera_WriteRegister(0x54ad,0x20);
	Camera_WriteRegister(0x54ae,0x00);
	Camera_WriteRegister(0x54af,0x16);
	
	
	Camera_WriteRegister(0x3406,0x00);//AWB MANUAL MODE
	
	Camera_WriteRegister(0x5192,0x04);//AWB REGISTER
	Camera_WriteRegister(0x5191,0xf8);
	Camera_WriteRegister(0x5193,0x70);
	Camera_WriteRegister(0x5194,0xf0);
	Camera_WriteRegister(0x5195,0xf0);
	Camera_WriteRegister(0x518d,0x3d);
	Camera_WriteRegister(0x518f,0x54);
	Camera_WriteRegister(0x518e,0x3d);
	Camera_WriteRegister(0x5190,0x54);
	Camera_WriteRegister(0x518b,0xc0);
	Camera_WriteRegister(0x518c,0xbd);
	Camera_WriteRegister(0x5187,0x18);
	Camera_WriteRegister(0x5188,0x18);
	Camera_WriteRegister(0x5189,0x6e);
	Camera_WriteRegister(0x518a,0x68);
	Camera_WriteRegister(0x5186,0x1c);
	Camera_WriteRegister(0x5181,0x50);
	Camera_WriteRegister(0x5184,0x25);
	Camera_WriteRegister(0x5182,0x11);
	Camera_WriteRegister(0x5183,0x14);
	Camera_WriteRegister(0x5184,0x25);
	Camera_WriteRegister(0x5185,0x24);
	
	Camera_WriteRegister(0x5025,0x82);//ISP CONTROL
	
	Camera_WriteRegister(0x3a0f,0x7e);//AEC/AGC
	Camera_WriteRegister(0x3a10,0x72);
	Camera_WriteRegister(0x3a1b,0x80);
	Camera_WriteRegister(0x3a1e,0x70);
	Camera_WriteRegister(0x3a11,0xd0);
	Camera_WriteRegister(0x3a1f,0x40);
	
	Camera_WriteRegister(0x5583,0x40);//SDE REGISTER . SPECIAL DIDITAL EFFECTS
	Camera_WriteRegister(0x5584,0x40);
	Camera_WriteRegister(0x5580,0x02);
	
	Camera_WriteRegister(0x3633,0x07);//ANALOG CONTROL REGISTER
	Camera_WriteRegister(0x3702,0x10);//ANALOG CONTROL REGISTER
	Camera_WriteRegister(0x3703,0xb2);//ANALOG CONTROL REGISTER
	Camera_WriteRegister(0x3704,0x18);//ANALOG CONTROL REGISTER
	Camera_WriteRegister(0x370b,0x40);//ANALOG CONTROL REGISTER
	Camera_WriteRegister(0x370d,0x02);//ANALOG CONTROL REGISTER
	Camera_WriteRegister(0x3620,0x52);//ANALOG CONTROL REGISTER
/*	
	Camera_WriteRegister(0x3808,0x04);//TIMING DVP OUTPUT HORIZONTAL WIDTH HIGH
	Camera_WriteRegister(0x3809,0x00);//TIMING DVP OUTPUT HORIZONTAL WIDTH LOW
	Camera_WriteRegister(0x380a,0x03);//TIMING DVP OUTPUT VERTICAL HEIGHT HIGH
	Camera_WriteRegister(0x380b,0x00);//TIMING DVP OUTPUT VERTICAL HEIGHT LOW
*/
////////////////////////////////////
//여기서 부터 문제발생	
////////////////////////////////////
	Camera_WriteRegister(0x3a00,0x78);//AEC/AGE
	
	Camera_WriteRegister(0x3818,0xA1); //TIMING CONTROL ,MIRROR OFF, VERTICAL SUBSAMPLE 1/2
	
	Camera_WriteRegister(0x3621,0xE0); //ARRAY CONTROL HORIZONTAL BINING DISABLE
	//Camera_WriteRegister(0x370d,0x40);//VERTICAL BINING DISABLE
	
	Camera_WriteRegister(0x4740,0x21);//DVP POLARITY CONTROL  중요

	
	//Camera_WriteRegister(0x5001,0xff);//ISP CONTROL  ENABLE : 매우 중요
	Camera_WriteRegister(0x5001,0xCF);//SCALE DISABLE 위쪽에도 선언되어 있음
	
	Camera_WriteRegister(0x5580,0x04);//SDE SPECIAL DIGITAL EFFECTS // CONTRAST ENABLE
	Camera_WriteRegister(0x5587,0x24);//SDE
	Camera_WriteRegister(0x5588,0x24);//SDE
	Camera_WriteRegister(0x558a,0x00);//SDE

	Camera_WriteRegister(0x5001,0xCF);//ISP CONTROL ENABLE ALL ENABLE // 중복선언
	Camera_WriteRegister(0x5583,0x40);//SDE  SPECIAL DIGITAL EPPECTS
	Camera_WriteRegister(0x5584,0x40);//SDE  SPECIAL DIGITAL EPPECTS
	Camera_WriteRegister(0x5580,0x02);//SDE  SPECIAL DIGITAL EPPECTS

	Camera_WriteRegister(0x5001,0xCF);//ISP CONTROL ENABLE ALL ENABLE // 중복선언
	Camera_WriteRegister(0x5589,0x10);//SDE  SPECIAL DIGITAL EPPECTS
	Camera_WriteRegister(0x5580,0x04);//SDE  SPECIAL DIGITAL EPPECTS
	Camera_WriteRegister(0x558a,0x00);//SDE  SPECIAL DIGITAL EPPECTS
 
	Camera_WriteRegister(0x3406,0x0 );//AWB AUTO MODE( AUTO WHITE BALANCE)
	Camera_WriteRegister(0x5192,0x04);//AWB ~
	Camera_WriteRegister(0x5191,0xf8);
	Camera_WriteRegister(0x518d,0x26);
	Camera_WriteRegister(0x518f,0x42);
	Camera_WriteRegister(0x518e,0x2b);
	Camera_WriteRegister(0x5190,0x42);
	Camera_WriteRegister(0x518b,0xd0);
	Camera_WriteRegister(0x518c,0xbd);                    
	Camera_WriteRegister(0x5187,0x18);
	Camera_WriteRegister(0x5188,0x18);
	Camera_WriteRegister(0x5189,0x56);
	Camera_WriteRegister(0x518a,0x5c);
	Camera_WriteRegister(0x5186,0x1c);
	Camera_WriteRegister(0x5181,0x50);
	Camera_WriteRegister(0x5184,0x20);
	Camera_WriteRegister(0x5182,0x11);
	Camera_WriteRegister(0x5183,0x0 );//AWB

	Camera_WriteRegister(0x3a0f,0x58);//AEC/AGC
	Camera_WriteRegister(0x3a10,0x50);
	Camera_WriteRegister(0x3a11,0x91);
	Camera_WriteRegister(0x3a1b,0x58);
	Camera_WriteRegister(0x3a1e,0x50);
	Camera_WriteRegister(0x3a1f,0x20);
    
	Camera_WriteRegister(0x5580,0x01);//RAW GAMMA
	Camera_WriteRegister(0x5581,0x6f);
	Camera_WriteRegister(0x5582,0x20);
	Camera_WriteRegister(0x558a,0x01);

	//Camera_WriteRegister(0x501e,0x2a);//ISP CONTROL :: 매우 중요 //DITHER 설정
	Camera_WriteRegister(0x5002,0x60);//ISP CONTROL 
	//Camera_WriteRegister(0x501f,0x01);//ISP CONTROL FORMAT MUX CONTROL ISP RGB 설정
	Camera_WriteRegister(0x501f,0x00);//ISP YUV	0X00  , CIF YUV422 0X06 상위에서 설정됨


	//1600 x1200
	Camera_WriteRegister(0x3808,0x06);//TIMING DVP OUTPUT HORIZONTAL WIDTH HIGH 
	Camera_WriteRegister(0x3809,0x40);//TIMING DVP OUTPUT HORIZONTAL WIDTH LOW
	Camera_WriteRegister(0x380a,0x04);//TIMING DVP OUTPUT VERTICAL HEIGHT HIGH
	Camera_WriteRegister(0x380b,0xB0);//TIMING DVP OUTPUT VERTICAL HEIGHT LOW	

	//1024 x 768
	/*
	Camera_WriteRegister(0x3808,0x04);//TIMING DVP OUTPUT HORIZONTAL WIDTH HIGH 
	Camera_WriteRegister(0x3809,0x00);//TIMING DVP OUTPUT HORIZONTAL WIDTH LOW
	Camera_WriteRegister(0x380a,0x03);//TIMING DVP OUTPUT VERTICAL HEIGHT HIGH
	Camera_WriteRegister(0x380b,0x00);//TIMING DVP OUTPUT VERTICAL HEIGHT LOW	
	*/
/*
	data=Camera_ReadRegister(0x3818);//ISP CONTROL REGISTER MIRROR VERTICAL FLIP ON
	Camera_WriteRegister(0x3818,(data|0x60) & 0xFF);//MIRROR ON, VERTICAL FLIP ON
*/


	
	Camera_WriteRegister(0x4300 ,0x32);//YUV 422, UYVY 설정
	Camera_WriteRegister(0x4730,0x01);//CCIR656 MODE ENABLE

	
}
void OV5642_Setting(void)
{
	//uint16_t data;
	
	Camera_WriteRegister(0x3103,0x93);//PCLK CLOCK SELECT 
	Camera_WriteRegister(0x3008,0x82);//RESET SOFTWARE
	//Camera_WriteRegister(0x3008,0x02);
	
	Camera_WriteRegister(0x3017,0x7f);//PAD OUTPUT ENABLE 01
	Camera_WriteRegister(0x3018,0xfc);//PAD OUTPUT ENABLE 02
	
	Camera_WriteRegister(0x3810,0xc2);//PLL CONTROL
	Camera_WriteRegister(0x3615,0xf0);//ANALOG CONTROL REGISTER
	
	Camera_WriteRegister(0x3000,0x00);//SYSTEM RESET 00
	Camera_WriteRegister(0x3001,0x00);//SYSTEM RESET 01
	Camera_WriteRegister(0x3002,0x00);//SYSTEM RESET 02
	Camera_WriteRegister(0x3003,0x00);//SYSTEM RESET 03
	
	Camera_WriteRegister(0x3000,0xf8);
	Camera_WriteRegister(0x3001,0x48);
	Camera_WriteRegister(0x3002,0x5c);
	Camera_WriteRegister(0x3003,0x02);
	Camera_WriteRegister(0x3004,0x07);
	Camera_WriteRegister(0x3005,0xff);
	Camera_WriteRegister(0x3006,0xff);
	Camera_WriteRegister(0x3007,0x07);
	Camera_WriteRegister(0x3011,0x08);
	
	Camera_WriteRegister(0x3010,0x10);//PLL CONTROL
	
	Camera_WriteRegister(0x460c,0x22);//PCLK 0X20 AUTOMODE  0X22 PCLK MANUAL MODE
	Camera_WriteRegister(0x3815,0x01);//PCLK MANUAL SET
	
			Camera_WriteRegister(0x370d,0x06);//ANALOG CONTROL REGISTER
			Camera_WriteRegister(0x370c,0xa0);//ANALOG CONTROL REGISTER
	
			Camera_WriteRegister(0x3602,0xfc);//ANALOG CONTROL REGISTER
			Camera_WriteRegister(0x3612,0xff);//ANALOG CONTROL REGISTER
			Camera_WriteRegister(0x3634,0xc0);//ANALOG CONTROL REGISTER
			Camera_WriteRegister(0x3613,0x00);//ANALOG CONTROL REGISTER
			Camera_WriteRegister(0x3605,0x7c);//ANALOG CONTROL REGISTER
			Camera_WriteRegister(0x3621,0x09);//ANALOG CONTROL REGISTER
			Camera_WriteRegister(0x3622,0x00);//ANALOG CONTROL REGISTER
			Camera_WriteRegister(0x3604,0x40);//ANALOG CONTROL REGISTER
			Camera_WriteRegister(0x3603,0xa7); //ANALOG CONTROL REGISTER
			Camera_WriteRegister(0x3603,0x27);//ANALOG CONTROL REGISTER
			
	Camera_WriteRegister(0x4000,0x21);//BLACK LEVEL CALIBRATION [BLC] SIGNAL ENABLE
	Camera_WriteRegister(0x401d,0x02);//BLC FRAME CTRL

			Camera_WriteRegister(0x3600,0x54);//ANALOG CONTROL REGISTER
			Camera_WriteRegister(0x3605,0x04);//ANALOG CONTROL REGISTER
			Camera_WriteRegister(0x3606,0x3f);//ANALOG CONTROL REGISTER
			
	Camera_WriteRegister(0x3c01,0x80);//LIGHT FREQUENCY REGISTER >> =AUTO DETECTION ENABLE
	
	Camera_WriteRegister(0x5000,0x4f);//ISP CONTROL 00
	Camera_WriteRegister(0x5020,0x04);//ISP RESERVED
	
	Camera_WriteRegister(0x5181,0x79);//AWB
	Camera_WriteRegister(0x5182,0x00);//AWB
	Camera_WriteRegister(0x5185,0x22);//AWB
	Camera_WriteRegister(0x5197,0x01);//AWB
	
	//Camera_WriteRegister(0x5001,0xff);//ISP CONTROL 01
	Camera_WriteRegister(0x5001,0xCF);//ISP CONTROL 01

	Camera_WriteRegister(0x5500,0x0a);//UV CONTROL
	Camera_WriteRegister(0x5504,0x00);//UV
	Camera_WriteRegister(0x5505,0x7f);//UV
	
	Camera_WriteRegister(0x5080,0x08);//ISP RESERVED
	
	Camera_WriteRegister(0x300e,0x18);//ISP MIPI CONTROL POWER DOWN RX,TX
	
	Camera_WriteRegister(0x4610,0x00);//NONE
	
	Camera_WriteRegister(0x471d,0x05);//DVP CONTROL
	Camera_WriteRegister(0x4708,0x06);//DVP
	
		Camera_WriteRegister(0x3710,0x10);//ANALOG CONTROL REGISTER
		Camera_WriteRegister(0x3632,0x41);//ANALOG CONTROL REGISTER
		Camera_WriteRegister(0x3702,0x40);//ANALOG CONTROL REGISTER
		Camera_WriteRegister(0x3620,0x37);//ANALOG CONTROL REGISTER
		Camera_WriteRegister(0x3631,0x01);//ANALOG CONTROL REGISTER
	
	Camera_WriteRegister(0x3808,0x02);//TIMING  DVPHO
	Camera_WriteRegister(0x3809,0x80);//TIMING  DVPHO
	Camera_WriteRegister(0x380a,0x01);//TIMING  DVPVO
	Camera_WriteRegister(0x380b,0xe0);//TIMING  DVPVO
	Camera_WriteRegister(0x380e,0x07);//TIMING  VTS
	Camera_WriteRegister(0x380f,0xd0);//TIMING  VTS

	//Camera_WriteRegister(0x501f,0x00); //POINT FORMAT MUX CONTROL // 설정값
	Camera_WriteRegister(0x501f,0x00); //POINT FORMAT MUX CONTROL // ISP  0-YUV,1- RGB, 2-YUV,3-RAW,4-CIF RAW,5-CIF RAW,6-CIF YUV
	
	//Camera_WriteRegister(0x5000,0x4f);//ISP CONTROL 00 //중복선언
	//Camera_WriteRegister(0x4300,0x30);//FORMAT :: OUTPUT 체크 하단부에서 선언
	
	Camera_WriteRegister(0x3503,0x07);//AEC PK MANUAL. VTS,AGC,AEC MANUAL ENABLE 자동노출
	Camera_WriteRegister(0x3501,0x73);//AEC
	Camera_WriteRegister(0x3502,0x80);//AEC
	Camera_WriteRegister(0x350b,0x00);//AEC
	Camera_WriteRegister(0x3503,0x07);//AEC
	
	Camera_WriteRegister(0x3824,0x11);//NONE
	
	Camera_WriteRegister(0x3501,0x1e);//AEC
	Camera_WriteRegister(0x3502,0x80);//AEC
	Camera_WriteRegister(0x350b,0x7f);//AEC
	
	
	//3200 * 2000
	Camera_WriteRegister(0x380c,0x0C);//TIMING  HTS
	Camera_WriteRegister(0x380d,0x80);//TIMING HTS
	Camera_WriteRegister(0x380e,0x07);//TIMING VTS // 중복설정 여기가 1000으로 설정이 되어 화면이 출력이 안된것으로 보임
	Camera_WriteRegister(0x380f,0xD0);//TIMING VTS //중복설정
	
	Camera_WriteRegister(0x3a0d,0x04);//POWER DOWN DOMAIN AEC/AGC
	Camera_WriteRegister(0x3a0e,0x03);
	
	//Camera_WriteRegister(0x3818,0xc1);//TIMING CONTROL REGISTER 18 매우중요
	//Camera_WriteRegister(0x3818,0xc0);//MIRROR ON ENABLE VERTICAL FLIP
	
	Camera_WriteRegister(0x3705,0xdb);//ANALOG CONTROL
	Camera_WriteRegister(0x370a,0x81);//ANALOG CONTROL
	
	Camera_WriteRegister(0x3800,0x01);//TIMING HREF HORIZONTAL START POINT HIGH
	Camera_WriteRegister(0x3801,0x8A);//TIMING HREF HORIZONTAL START POINT LOW
	
	//Camera_WriteRegister(0x3621,0xc7);//SENSOR CONTROL ARRAY  HORIZONTAL BINING ENABLE : 매우중요
	Camera_WriteRegister(0x3621,0x10);//HORIZONTAL BINNING DISABLE
	
	Camera_WriteRegister(0x3802,0x00);//TIMING HREF VERTICAL START POINT LOW
	Camera_WriteRegister(0x3803,0x0A);//TIMING HREF VERTICAL START POINT LOW
	
	Camera_WriteRegister(0x3827,0x08);//NONE
	
	Camera_WriteRegister(0x3810,0xc0);//TIMING HORIZONTAL AND VERTICAL OFFSET SETING : 매우 중요
	
	Camera_WriteRegister(0x3804,0x06);// TIMING HREF HORIZONTAL WIDTH HIGH
	Camera_WriteRegister(0x3805,0x40);//TIMING HREF HORIZONTAL WIDTH LOW
	//Camera_WriteRegister(0x5682,0x06);//SCALE HORIZONTAL END POSITION FOR AVERAGE WINDOW
	//Camera_WriteRegister(0x5683,0x40);//SCALE HORIZONTAL END POSITION FOR AVERAGE WINDOW
	
	Camera_WriteRegister(0x3806,0x04);//TIMING HREF VERTICAL HEIGHT HIGH 
	Camera_WriteRegister(0x3807,0xb0);//TIMING HREF VERTICAL HEIGHT LOW
	//Camera_WriteRegister(0x5686,0x04);//SCALE VERTICAL END POSITION FOR AVERAGE WINDOW
	//Camera_WriteRegister(0x5687,0xb0);//SCALE VERTICAL END POSITION FOR AVERAGE WINDOW
	
	Camera_WriteRegister(0x3a00,0x78);//AEC CONTROL 00 LESS ONE LINE MODE, BAND FUNCTION, BAND LOW LIMIT MODE
	Camera_WriteRegister(0x3a1a,0x04);//AEC CONTROL REGISTER
	Camera_WriteRegister(0x3a13,0x30);//AEC CONTROL REGISTER
	Camera_WriteRegister(0x3a18,0x00);//AEC CONTROL REGISTER
	Camera_WriteRegister(0x3a19,0x7c);//AEC CONTROL REGISTER
	Camera_WriteRegister(0x3a08,0x12);//AEC CONTROL REGISTER
	Camera_WriteRegister(0x3a09,0xc0);//AEC CONTROL REGISTER
	Camera_WriteRegister(0x3a0a,0x0f);//AEC CONTROL REGISTER
	Camera_WriteRegister(0x3a0b,0xa0);//AEC CONTROL REGISTER
	
	Camera_WriteRegister(0x3004,0xff);//CLOCK ENABLE ALL ENABLE : 재설정 하는이유?
	
	Camera_WriteRegister(0x350c,0x07);//AEC VTS HIGH BITS
	Camera_WriteRegister(0x350d,0xd0);//AEC VTS LOW BITS
	Camera_WriteRegister(0x3500,0x00);//AEC EXPOSURE(노출설정)
	Camera_WriteRegister(0x3501,0x00);//AEC CONTROL REGISTER
	Camera_WriteRegister(0x3502,0x00);//AEC CONTROL REGISTER
	Camera_WriteRegister(0x350a,0x00);//AEC CONTROL REGISTER
	Camera_WriteRegister(0x350b,0x00);//AEC CONTROL REGISTER
	Camera_WriteRegister(0x3503,0x00);//AEC CONTROL REGISTER
	
	Camera_WriteRegister(0x528a,0x02);//DE-NOISE
	Camera_WriteRegister(0x528b,0x04);//DE-NOISE
	Camera_WriteRegister(0x528c,0x08);//DE-NOISE
	Camera_WriteRegister(0x528d,0x08);//DE-NOISE
	Camera_WriteRegister(0x528e,0x08);//DE-NOISE
	Camera_WriteRegister(0x528f,0x10);//DE-NOISE
	Camera_WriteRegister(0x5290,0x10);//DE-NOISE
	Camera_WriteRegister(0x5292,0x00);//DE-NOISE
	Camera_WriteRegister(0x5293,0x02);//DE-NOISE
	Camera_WriteRegister(0x5294,0x00);//DE-NOISE
	Camera_WriteRegister(0x5295,0x02);//DE-NOISE
	Camera_WriteRegister(0x5296,0x00);//DE-NOISE
	Camera_WriteRegister(0x5297,0x02);//DE-NOISE
	Camera_WriteRegister(0x5298,0x00);//DE-NOISE
	Camera_WriteRegister(0x5299,0x02);//DE-NOISE
	Camera_WriteRegister(0x529a,0x00);//DE-NOISE
	Camera_WriteRegister(0x529b,0x02);//DE-NOISE
	Camera_WriteRegister(0x529c,0x00);//DE-NOISE
	Camera_WriteRegister(0x529d,0x02);//DE-NOISE
	Camera_WriteRegister(0x529e,0x00);//DE-NOISE
	Camera_WriteRegister(0x529f,0x02);//DE-NOISE
	
	Camera_WriteRegister(0x3a0f,0x3c);//AEC/AGE
	Camera_WriteRegister(0x3a10,0x30);//AEC/AGE
	Camera_WriteRegister(0x3a1b,0x3c);//AEC/AGE
	Camera_WriteRegister(0x3a1e,0x30);//AEC/AGE
	Camera_WriteRegister(0x3a11,0x70);//AEC/AGE
	Camera_WriteRegister(0x3a1f,0x10);//AEC/AGE
	
	Camera_WriteRegister(0x3030,0x0b);//PWC CONTORL 초기값
	
	Camera_WriteRegister(0x3a02,0x00);//AEC/AGE
	Camera_WriteRegister(0x3a03,0x7d);//AEC/AGE
	Camera_WriteRegister(0x3a04,0x00);//AEC/AGE
	Camera_WriteRegister(0x3a14,0x00);//AEC/AGE
	Camera_WriteRegister(0x3a15,0x7d);//AEC/AGE
	Camera_WriteRegister(0x3a16,0x00);//AEC/AGE
	Camera_WriteRegister(0x3a00,0x7c);//AEC/AGE
	Camera_WriteRegister(0x3a08,0x09);//AEC/AGE
	Camera_WriteRegister(0x3a09,0x60);//AEC/AGE
	Camera_WriteRegister(0x3a0a,0x07);//AEC/AGE
	Camera_WriteRegister(0x3a0b,0xd0);//AEC/AGE
	Camera_WriteRegister(0x3a0d,0x08);//AEC/AGE
	Camera_WriteRegister(0x3a0e,0x06);//AEC/AGE
	
	Camera_WriteRegister(0x5193,0x70);//AWB AUTOMATIC WHITE BLANCE
	
	Camera_WriteRegister(0x3620,0x57);//ANALOG CONTROL REGISTER
	Camera_WriteRegister(0x3703,0x98);//ANALOG CONTROL REGISTER
	Camera_WriteRegister(0x3704,0x1c);//ANALOG CONTROL REGISTER
	
	Camera_WriteRegister(0x589b,0x04);//LENC
	Camera_WriteRegister(0x589a,0xc5);//LENC
	
	Camera_WriteRegister(0x528a,0x00);//DE-NOISE
	Camera_WriteRegister(0x528b,0x02);//DE-NOISE
	Camera_WriteRegister(0x528c,0x08);//DE-NOISE
	Camera_WriteRegister(0x528d,0x10);//DE-NOISE
	Camera_WriteRegister(0x528e,0x20);//DE-NOISE
	Camera_WriteRegister(0x528f,0x28);//DE-NOISE
	Camera_WriteRegister(0x5290,0x30);//DE-NOISE
	Camera_WriteRegister(0x5292,0x00);//DE-NOISE
	Camera_WriteRegister(0x5293,0x00);//DE-NOISE
	Camera_WriteRegister(0x5294,0x00);//DE-NOISE
	Camera_WriteRegister(0x5295,0x02);//DE-NOISE
	Camera_WriteRegister(0x5296,0x00);//DE-NOISE
	Camera_WriteRegister(0x5297,0x08);//DE-NOISE
	Camera_WriteRegister(0x5298,0x00);//DE-NOISE
	Camera_WriteRegister(0x5299,0x10);//DE-NOISE
	Camera_WriteRegister(0x529a,0x00);//DE-NOISE
	Camera_WriteRegister(0x529b,0x20);//DE-NOISE
	Camera_WriteRegister(0x529c,0x00);//DE-NOISE
	Camera_WriteRegister(0x529d,0x28);//DE-NOISE
	Camera_WriteRegister(0x529e,0x00);//DE-NOISE
	Camera_WriteRegister(0x529f,0x30);//DE-NOISE
	Camera_WriteRegister(0x5282,0x00);//DE-NOISE
	Camera_WriteRegister(0x5300,0x00);//DE-NOISE
	Camera_WriteRegister(0x5301,0x20);//DE-NOISE
	Camera_WriteRegister(0x5302,0x00);//DE-NOISE
	Camera_WriteRegister(0x5303,0x7c);//DE-NOISE
	Camera_WriteRegister(0x530c,0x00);//DE-NOISE
	Camera_WriteRegister(0x530d,0x0c);//DE-NOISE
	Camera_WriteRegister(0x530e,0x20);//DE-NOISE
	Camera_WriteRegister(0x530f,0x80);//DE-NOISE
	Camera_WriteRegister(0x5310,0x20);//DE-NOISE
	Camera_WriteRegister(0x5311,0x80);//DE-NOISE
	Camera_WriteRegister(0x5308,0x20);//DE-NOISE
	
	Camera_WriteRegister(0x5309,0x40);//CIP - COLOR INTERPOLATION
	Camera_WriteRegister(0x5304,0x00);//CIP - COLOR INTERPOLATION
	Camera_WriteRegister(0x5305,0x30);//CIP - COLOR INTERPOLATION
	Camera_WriteRegister(0x5306,0x00);//CIP - COLOR INTERPOLATION
	Camera_WriteRegister(0x5307,0x80);//CIP - COLOR INTERPOLATION
	Camera_WriteRegister(0x5314,0x08);//CIP - COLOR INTERPOLATION
	Camera_WriteRegister(0x5315,0x20);//CIP - COLOR INTERPOLATION
	Camera_WriteRegister(0x5319,0x30);//CIP - COLOR INTERPOLATION
	Camera_WriteRegister(0x5316,0x10);//CIP - COLOR INTERPOLATION
	Camera_WriteRegister(0x5317,0x08);//CIP - COLOR INTERPOLATION
	Camera_WriteRegister(0x5318,0x02);//CIP - COLOR INTERPOLATION
	
	Camera_WriteRegister(0x5380,0x01);//CMX COLOR MATRIX
	Camera_WriteRegister(0x5381,0x00);//CMX COLOR MATRIX
	Camera_WriteRegister(0x5382,0x00);//CMX COLOR MATRIX
	Camera_WriteRegister(0x5383,0x4e);//CMX COLOR MATRIX
	Camera_WriteRegister(0x5384,0x00);//CMX COLOR MATRIX
	Camera_WriteRegister(0x5385,0x0f);//CMX COLOR MATRIX
	Camera_WriteRegister(0x5386,0x00);//CMX COLOR MATRIX
	Camera_WriteRegister(0x5387,0x00);//CMX COLOR MATRIX
	Camera_WriteRegister(0x5388,0x01);//CMX COLOR MATRIX
	Camera_WriteRegister(0x5389,0x15);//CMX COLOR MATRIX
	Camera_WriteRegister(0x538a,0x00);//CMX COLOR MATRIX
	Camera_WriteRegister(0x538b,0x31);//CMX COLOR MATRIX
	Camera_WriteRegister(0x538c,0x00);//CMX COLOR MATRIX
	Camera_WriteRegister(0x538d,0x00);//CMX COLOR MATRIX
	Camera_WriteRegister(0x538e,0x00);//CMX COLOR MATRIX
	Camera_WriteRegister(0x538f,0x0f);//CMX COLOR MATRIX
	Camera_WriteRegister(0x5390,0x00);//CMX COLOR MATRIX
	Camera_WriteRegister(0x5391,0xab);//CMX COLOR MATRIX
	Camera_WriteRegister(0x5392,0x00);//CMX COLOR MATRIX
	Camera_WriteRegister(0x5393,0xa2);//CMX COLOR MATRIX
	Camera_WriteRegister(0x5394,0x08);//CMX COLOR MATRIX
	
	
	Camera_WriteRegister(0x5480,0x14);//GAMMA
	Camera_WriteRegister(0x5481,0x21);
	Camera_WriteRegister(0x5482,0x36);
	Camera_WriteRegister(0x5483,0x57);
	Camera_WriteRegister(0x5484,0x65);
	Camera_WriteRegister(0x5485,0x71);
	Camera_WriteRegister(0x5486,0x7d);
	Camera_WriteRegister(0x5487,0x87);
	Camera_WriteRegister(0x5488,0x91);
	Camera_WriteRegister(0x5489,0x9a);
	Camera_WriteRegister(0x548a,0xaa);
	Camera_WriteRegister(0x548b,0xb8);
	Camera_WriteRegister(0x548c,0xcd);
	Camera_WriteRegister(0x548d,0xdd);
	Camera_WriteRegister(0x548e,0xea);
	Camera_WriteRegister(0x548f,0x10);
	
	
	Camera_WriteRegister(0x5490,0x05);//UV GAMMA 
	Camera_WriteRegister(0x5491,0x00);
	Camera_WriteRegister(0x5492,0x04);
	Camera_WriteRegister(0x5493,0x20);
	Camera_WriteRegister(0x5494,0x03);
	Camera_WriteRegister(0x5495,0x60);
	Camera_WriteRegister(0x5496,0x02);
	Camera_WriteRegister(0x5497,0xb8);
	Camera_WriteRegister(0x5498,0x02);
	Camera_WriteRegister(0x5499,0x86);
	Camera_WriteRegister(0x549a,0x02);
	Camera_WriteRegister(0x549b,0x5b);
	Camera_WriteRegister(0x549c,0x02);
	Camera_WriteRegister(0x549d,0x3b);
	Camera_WriteRegister(0x549e,0x02);
	Camera_WriteRegister(0x549f,0x1c);
	Camera_WriteRegister(0x54a0,0x02);
	Camera_WriteRegister(0x54a1,0x04);
	Camera_WriteRegister(0x54a2,0x01);
	Camera_WriteRegister(0x54a3,0xed);
	Camera_WriteRegister(0x54a4,0x01);
	Camera_WriteRegister(0x54a5,0xc5);
	Camera_WriteRegister(0x54a6,0x01);
	Camera_WriteRegister(0x54a7,0xa5);
	Camera_WriteRegister(0x54a8,0x01);
	Camera_WriteRegister(0x54a9,0x6c);
	Camera_WriteRegister(0x54aa,0x01);
	Camera_WriteRegister(0x54ab,0x41);
	Camera_WriteRegister(0x54ac,0x01);
	Camera_WriteRegister(0x54ad,0x20);
	Camera_WriteRegister(0x54ae,0x00);
	Camera_WriteRegister(0x54af,0x16);
	
	
	Camera_WriteRegister(0x3406,0x00);//AWB MANUAL MODE
	
	Camera_WriteRegister(0x5192,0x04);//AWB REGISTER
	Camera_WriteRegister(0x5191,0xf8);
	Camera_WriteRegister(0x5193,0x70);
	Camera_WriteRegister(0x5194,0xf0);
	Camera_WriteRegister(0x5195,0xf0);
	Camera_WriteRegister(0x518d,0x3d);
	Camera_WriteRegister(0x518f,0x54);
	Camera_WriteRegister(0x518e,0x3d);
	Camera_WriteRegister(0x5190,0x54);
	Camera_WriteRegister(0x518b,0xc0);
	Camera_WriteRegister(0x518c,0xbd);
	Camera_WriteRegister(0x5187,0x18);
	Camera_WriteRegister(0x5188,0x18);
	Camera_WriteRegister(0x5189,0x6e);
	Camera_WriteRegister(0x518a,0x68);
	Camera_WriteRegister(0x5186,0x1c);
	Camera_WriteRegister(0x5181,0x50);
	Camera_WriteRegister(0x5184,0x25);
	Camera_WriteRegister(0x5182,0x11);
	Camera_WriteRegister(0x5183,0x14);
	Camera_WriteRegister(0x5184,0x25);
	Camera_WriteRegister(0x5185,0x24);
	
	Camera_WriteRegister(0x5025,0x82);//ISP CONTROL
	
	Camera_WriteRegister(0x3a0f,0x7e);//AEC/AGC
	Camera_WriteRegister(0x3a10,0x72);
	Camera_WriteRegister(0x3a1b,0x80);
	Camera_WriteRegister(0x3a1e,0x70);
	Camera_WriteRegister(0x3a11,0xd0);
	Camera_WriteRegister(0x3a1f,0x40);
	
	Camera_WriteRegister(0x5583,0x40);//SDE REGISTER . SPECIAL DIDITAL EFFECTS
	Camera_WriteRegister(0x5584,0x40);
	Camera_WriteRegister(0x5580,0x02);
	
	Camera_WriteRegister(0x3633,0x07);//ANALOG CONTROL REGISTER
	Camera_WriteRegister(0x3702,0x10);//ANALOG CONTROL REGISTER
	Camera_WriteRegister(0x3703,0xb2);//ANALOG CONTROL REGISTER
	Camera_WriteRegister(0x3704,0x18);//ANALOG CONTROL REGISTER
	Camera_WriteRegister(0x370b,0x40);//ANALOG CONTROL REGISTER
	Camera_WriteRegister(0x370d,0x02);//ANALOG CONTROL REGISTER
	Camera_WriteRegister(0x3620,0x52);//ANALOG CONTROL REGISTER
/*	
	Camera_WriteRegister(0x3808,0x04);//TIMING DVP OUTPUT HORIZONTAL WIDTH HIGH
	Camera_WriteRegister(0x3809,0x00);//TIMING DVP OUTPUT HORIZONTAL WIDTH LOW
	Camera_WriteRegister(0x380a,0x03);//TIMING DVP OUTPUT VERTICAL HEIGHT HIGH
	Camera_WriteRegister(0x380b,0x00);//TIMING DVP OUTPUT VERTICAL HEIGHT LOW
*/
	Camera_WriteRegister(0x3a00,0x78);//AEC/AGE
	
	Camera_WriteRegister(0x3818,0x01); //TIMING CONTROL ,MIRROR OFF, VERTICAL SUBSAMPLE 1/2
	
	Camera_WriteRegister(0x3621,0xE0); //ARRAY CONTROL HORIZONTAL BINING DISABLE
	//Camera_WriteRegister(0x370d,0x40);//VERTICAL BINING DISABLE
	
	Camera_WriteRegister(0x4740,0x21);//DVP POLARITY CONTROL  중요

	
	////////////////////////////////////
	// 
	////////////////////////////////////
	Camera_WriteRegister(0x5001,0xCF);//SCALE DISABLE 위쪽에도 선언되어 있음
	Camera_WriteRegister(0x5002,0x70);//ISP CONTROL 
	Camera_WriteRegister(0x5003,0xA8);
	Camera_WriteRegister(0x501E,0x00);//RGD DITHER CONTROL OFF
	Camera_WriteRegister(0x501f,0x00);//ISP YUV	0X00  , CIF YUV422 0X06 상위에서 설정됨


	Camera_WriteRegister(0x5580,0x04);//SDE SPECIAL DIGITAL EFFECTS // CONTRAST ENABLE
	Camera_WriteRegister(0x5587,0x24);//SDE
	Camera_WriteRegister(0x5588,0x24);//SDE
	Camera_WriteRegister(0x558a,0x00);//SDE

	Camera_WriteRegister(0x5001,0xCF);//ISP CONTROL ENABLE ALL ENABLE // 중복선언
	Camera_WriteRegister(0x5583,0x40);//SDE  SPECIAL DIGITAL EPPECTS
	Camera_WriteRegister(0x5584,0x40);//SDE  SPECIAL DIGITAL EPPECTS
	Camera_WriteRegister(0x5580,0x02);//SDE  SPECIAL DIGITAL EPPECTS

	Camera_WriteRegister(0x5001,0xCF);//ISP CONTROL ENABLE ALL ENABLE // 중복선언
	Camera_WriteRegister(0x5589,0x10);//SDE  SPECIAL DIGITAL EPPECTS
	Camera_WriteRegister(0x5580,0x04);//SDE  SPECIAL DIGITAL EPPECTS
	Camera_WriteRegister(0x558a,0x00);//SDE  SPECIAL DIGITAL EPPECTS
 
	Camera_WriteRegister(0x3406,0x0 );//AWB AUTO MODE( AUTO WHITE BALANCE)
	Camera_WriteRegister(0x5192,0x04);//AWB ~
	Camera_WriteRegister(0x5191,0xf8);
	Camera_WriteRegister(0x518d,0x26);
	Camera_WriteRegister(0x518f,0x42);
	Camera_WriteRegister(0x518e,0x2b);
	Camera_WriteRegister(0x5190,0x42);
	Camera_WriteRegister(0x518b,0xd0);
	Camera_WriteRegister(0x518c,0xbd);                    
	Camera_WriteRegister(0x5187,0x18);
	Camera_WriteRegister(0x5188,0x18);
	Camera_WriteRegister(0x5189,0x56);
	Camera_WriteRegister(0x518a,0x5c);
	Camera_WriteRegister(0x5186,0x1c);
	Camera_WriteRegister(0x5181,0x50);
	Camera_WriteRegister(0x5184,0x20);
	Camera_WriteRegister(0x5182,0x11);
	Camera_WriteRegister(0x5183,0x0 );//AWB

	Camera_WriteRegister(0x3a0f,0x58);//AEC/AGC
	Camera_WriteRegister(0x3a10,0x50);
	Camera_WriteRegister(0x3a11,0x91);
	Camera_WriteRegister(0x3a1b,0x58);
	Camera_WriteRegister(0x3a1e,0x50);
	Camera_WriteRegister(0x3a1f,0x20);
    
	Camera_WriteRegister(0x5580,0x01);//RAW GAMMA
	Camera_WriteRegister(0x5581,0x6f);
	Camera_WriteRegister(0x5582,0x20);
	Camera_WriteRegister(0x558a,0x01);

	




	//1600 x1200
	Camera_WriteRegister(0x3808,0x06);//TIMING DVP OUTPUT HORIZONTAL WIDTH HIGH 
	Camera_WriteRegister(0x3809,0x40);//TIMING DVP OUTPUT HORIZONTAL WIDTH LOW
	Camera_WriteRegister(0x380a,0x04);//TIMING DVP OUTPUT VERTICAL HEIGHT HIGH
	Camera_WriteRegister(0x380b,0xB0);//TIMING DVP OUTPUT VERTICAL HEIGHT LOW	

	//1024 x 768
	/*
	Camera_WriteRegister(0x3808,0x04);//TIMING DVP OUTPUT HORIZONTAL WIDTH HIGH 
	Camera_WriteRegister(0x3809,0x00);//TIMING DVP OUTPUT HORIZONTAL WIDTH LOW
	Camera_WriteRegister(0x380a,0x03);//TIMING DVP OUTPUT VERTICAL HEIGHT HIGH
	Camera_WriteRegister(0x380b,0x00);//TIMING DVP OUTPUT VERTICAL HEIGHT LOW	
	*/
/*
	data=Camera_ReadRegister(0x3818);//ISP CONTROL REGISTER MIRROR VERTICAL FLIP ON
	Camera_WriteRegister(0x3818,(data|0x60) & 0xFF);//MIRROR ON, VERTICAL FLIP ON
*/


	
	Camera_WriteRegister(0x4300 ,0x32);//YUV 422, UYVY 설정
	Camera_WriteRegister(0x4730,0x01);//CCIR656 MODE ENABLE

	
}
#define OV5642_TOTAL_WIDTH	            3200
#define OV5642_TOTAL_HEIGHT	            2000
#define OV5642_SENSOR_SIZE_X	        2592
#define OV5642_SENSOR_SIZE_Y	            1944

int CameraView_1600_1200( void )
{
	uint32_t		width=0;
	uint32_t		height=0;
	uint32_t		width2=0;
	uint32_t		height2=0;
	
	uint8_t 		start_x_high = 0;
	uint8_t 		start_x_low  = 0;
	uint8_t 		start_y_high = 0;
	uint8_t 		start_y_low  = 0;

	uint8_t 		width_high   = 0;
	uint8_t 		width_low    = 0;
	uint8_t 		height_high  = 0;
	uint8_t 		height_low   = 0;

	uint8_t 		width_high2   = 0;
	uint8_t 		width_low2    = 0;
	uint8_t 		height_high2  = 0;
	uint8_t 		height_low2   = 0;
	
	
	uint8_t 		total_width_high  = 0;
	uint8_t 		total_width_low   = 0;
	uint8_t 		total_height_high = 0;
	uint8_t 		total_height_low  = 0;
	
	width2 = 1600;
	height2= 1200;
	width = 1600;
	height=	1200;


	start_x_high = ((OV5642_SENSOR_SIZE_X - width) / 2) >> 8;
	start_x_low  = ((OV5642_SENSOR_SIZE_X - width) / 2) & 0xff;
	start_y_high = ((OV5642_SENSOR_SIZE_Y - height) / 2) >> 8;
	start_y_low  = ((OV5642_SENSOR_SIZE_Y - height) / 2) & 0xff;
	
	width_high   = width  >> 8&0x00FF;
	width_low    = width  & 0xff;
	height_high  = height >> 8&0x00FF;
	height_low   = height & 0xff;

	width_high2   = width2  >> 8&0x00FF;
	width_low2    = width2  & 0xff;
	height_high2  = height2 >> 8&0x00FF;
	height_low2   = height2 & 0xff;


	total_width_high  = 3200  >> 8 &0x00FF;
	total_width_low   = 3200  & 0xff;
	total_height_high = 2000 >> 8&0x00FF;
	total_height_low  = 2000 & 0xff;

	//Dprintf("START POINT %02x, %02x, %02x, %02x \n",start_x_high,start_x_low,start_y_high,start_y_low);
	VGA_640_480_PreView();		
	
	Camera_WriteRegister(0x3810 ,0xc2);
	//Delay(1000);
	Camera_WriteRegister(0x3815 ,0x01);//1600.1200

	Camera_WriteRegister(0x3800 ,start_x_high );
	Camera_WriteRegister(0x3801 ,start_x_low);
	Camera_WriteRegister(0x3802 ,start_y_high );
	Camera_WriteRegister(0x3803 ,start_y_low );

	Camera_WriteRegister(0x3804 ,width_high );
	Camera_WriteRegister(0x3805 ,width_low);
	Camera_WriteRegister(0x3806 ,height_high);
	Camera_WriteRegister(0x3807 ,height_low);

	Camera_WriteRegister(0x3808 ,width_high2 );
	Camera_WriteRegister(0x3809 ,width_low2);
	Camera_WriteRegister(0x380A ,height_high2);
	Camera_WriteRegister(0x380B ,height_low2);
	//TOTAL WIDTH,HEIGHT, SIZE
	Camera_WriteRegister(0x380c ,total_width_high);
	Camera_WriteRegister(0x380d ,total_width_low);
	Camera_WriteRegister(0x380e ,total_height_high);
	Camera_WriteRegister(0x380f ,total_height_low);
	

	Camera_WriteRegister(0x4300 ,0x32);//YUV 422, UYVY 설정
	Camera_WriteRegister(0x4730,0x01);//CCIR656 MODE ENABLE

	return (0);
}
void BmpPlot(U16 x, U16 y, U16 color);
//unsigned short int (*ConversionData1)[VIDEO_CAMERA_VIEW_Y];
//unsigned short int (*ConversionData2)[VIDEO_CAMERA_VIEW_Y];

/*
uint16_t	**VRAM_FileBuf;

*VRAM_FileBuf = (uint16_t *)0x01000000;

VRAM_FileBuf[0][0] = 120;
VRAM_FileBuf[0][1] = 121;
*/

void EdgeDetection_Display(uint16_t sx, uint16_t sy, uint16_t dx, uint16_t dy, uint16_t width, uint16_t height, uint32_t addr)
{
	uint32_t		y = 0;
	uint32_t		x = 0;
	uint32_t		ex = 0;
	uint32_t		ey = 0;
	uint32_t		address = addr;
	uint16_t		(*to)[1024];
	to 			= (uint16_t(*)[1024])address;
	
	SetGraphicsPage(LCD_SCREEN);
	ey=dy;
	for(y=sy; y<=height; y++) 
	{
		ex=dx;
		for(x=sx; x<=width; x++)
		{
			BmpPlot( ex+dx, ey,to[y][x]);			
			ex++;
		}
		ey++;
	}
	//Dprintf("EdgeDetection_Display \n");
}
void EdgeDetection_Laplacian(uint16_t dx, uint16_t dy, uint16_t width, uint16_t height, uint32_t addr, uint16_t display)
{
	uint16_t 	centerValue1=0;
	uint16_t 	sum=0;
	uint16_t		i = 0;
	uint16_t		j = 0;
	uint16_t		x = 0;
	uint16_t		y = 0;
	uint16_t		row = 0;
	uint16_t		column = 0;		
	int mask[3][3]={ -1,-1,-1,
		             -1, 8,-1,
	                 -1,-1,-1 };

	uint32_t		View_Y = height;
	uint32_t		View_X = width;
	uint16_t			(*form)[1024];
	uint16_t			(*to)[1024];
	form 		= (uint16_t(*)[1024])USER_SCREEN2_ADDR;
	to 			= (uint16_t(*)[1024])USER_SCREEN3_ADDR;

	SetGraphicsPage(LCD_SCREEN);	
	for( row = View_Y; row > 0; row-- )
	{
		for( column = 0; column < View_X; column++ )
		{
			for(i=0; i<3; i++)
			{
				for(j=0; j<3; j++)
				{
					centerValue1 += form[j+row][i+column] * mask[i][j];	
				}
			}
			sum = abs(centerValue1);
			
			if( sum > 255 ) sum = 255;
			to[row+1][column+1]=(unsigned char)sum;
			
			centerValue1=0;
			sum=0;
		}
	}
    if(display == ON)
	{
		for(y=dy; y<=View_Y; y++) 
		{
			for(x=dx; x<=View_X; x++)
			{
				BmpPlot( x, y,to[y][x]);			
			}
		}
	}
}
/**
 * Converts the image to B&W.
 * The Mean() function can be used for calculating the optimal threshold.
 * \param level: the lightness threshold.
 * \return true if everything is ok
 */
uint16_t CxImageThreshold(uint16_t dx, uint16_t dy, uint16_t width, uint16_t height, uint32_t addr, uint16_t display, uint16_t level)
{
	uint32_t		y,x;
	//GrayScale();
	//CxImage tmp(head.biWidth,head.biHeight,1);
	//if (!tmp.IsValid()) return false;
	uint32_t		View_Y = height;
	uint32_t		View_X = width;
	uint16_t			(*form)[1024];
	uint16_t			(*to)[1024];
	form 		= (uint16_t(*)[1024])USER_SCREEN2_ADDR;
	to 			= (uint16_t(*)[1024])USER_SCREEN3_ADDR;

	SetGraphicsPage(LCD_SCREEN);	
	for (y=0;y<View_Y;y++)
	{
		//info.nProgress = (long)(100*y/768);
		//if (info.nEscape) break;
		for (x=0;x<View_X;x++)
		{
			if (form[y][x]>level)
			{
				to[y][x]=RGB565CONVERT(255,  255,    255);//BLACK
			}
			else
			{
				to[y][x]=0;
			}
		}
	}
    if(display == ON)
	{
		for(y=dy; y<=View_Y; y++) 
		{
			for(x=dx; x<=View_X; x++)
			{
				BmpPlot( x, y,to[y][x]);			
			}
		}
	}
	
	//tmp.SetPaletteColor(0,0,0,0);
	//tmp.SetPaletteColor(1,255,255,255);
	//Transfer(tmp);
	return true;
}

//#define  MIN(a, b)   (((a)<(b))?(a):(b)) 
#define  MAX(a, b)   (((a)>(b))?(a):(b)) 
uint16_t CxImageGamma(uint16_t dx, uint16_t dy, uint16_t width, uint16_t height, uint32_t addr, uint16_t display, float gamma)
{
/*
	double dinvgamma = 1/gamma;
	double dMax = pow(255.0, dinvgamma) / 255.0;
	uint16_t		row = 0;
	uint16_t		column = 0;		
	uint16_t		x = 0;
	uint16_t		y = 0;

	uint32_t		View_Y = height;
	uint32_t		View_X = width;
	uint16_t			(*form)[1024];
	uint16_t			(*to)[1024];
	form 		= (uint16_t(*)[1024])USER_SCREEN2_ADDR;
	to 			= (uint16_t(*)[1024])USER_SCREEN3_ADDR;
	Dprintf("Gamma Start\n");
	for( row = View_Y; row > 0; row-- )
	{
		for( column = 0; column < View_X; column++ )
		{
			to[row][column] = MAX(0,(uint16_t)MIN(255,(uint16_t) pow((double)form[row][column], dinvgamma) / dMax));
		}
	}
	Dprintf("Gamma End\n");
    if(display == ON)
	{
		for(y=dy; y<=View_Y; y++) 
		{
			for(x=dx; x<=View_X; x++)
			{
				BmpPlot( x, y,to[y][x]);			
			}
		}
	}
	//CxImage::Lut(BYTE* pLut)  함수를 호출하여 덮어준다.
	return true;
*/	
}
#if 0 
bool CxImage::Lut(BYTE* pLut)
{
	if (!pDib || !pLut) return false;
	RGBQUAD color;

	double dbScaler;
	if (head.biClrUsed==0){

		long xmin,xmax,ymin,ymax;
		if (pSelection){
			xmin = info.rSelectionBox.left; xmax = info.rSelectionBox.right;
			ymin = info.rSelectionBox.bottom; ymax = info.rSelectionBox.top;
		} else {
			// faster loop for full image
			BYTE *iSrc=info.pImage;
			for(unsigned long i=0; i < head.biSizeImage ; i++){
				*iSrc++ = pLut[*iSrc];
			}
			return true;
		}

		dbScaler = 100.0/ymax;

		for(long y=ymin; y<ymax; y++){
			info.nProgress = (long)(y*dbScaler); //<Anatoly Ivasyuk>
			for(long x=xmin; x<xmax; x++){
#if CXIMAGE_SUPPORT_SELECTION
				if (SelectionIsInside(x,y))
#endif //CXIMAGE_SUPPORT_SELECTION
				{
					color = GetPixelColor(x,y);
					color.rgbRed = pLut[color.rgbRed];
					color.rgbGreen = pLut[color.rgbGreen];
					color.rgbBlue = pLut[color.rgbBlue];
					SetPixelColor(x,y,color);
				}
			}
		}
#if CXIMAGE_SUPPORT_SELECTION
	} else if (pSelection && (head.biBitCount==8) && IsGrayScale()){
		long xmin,xmax,ymin,ymax;
		xmin = info.rSelectionBox.left; xmax = info.rSelectionBox.right;
		ymin = info.rSelectionBox.bottom; ymax = info.rSelectionBox.top;

		dbScaler = 100.0/ymax;
		for(long y=ymin; y<ymax; y++){
			info.nProgress = (long)(y*dbScaler);
			for(long x=xmin; x<xmax; x++){
				if (SelectionIsInside(x,y))
				{
					SetPixelIndex(x,y,pLut[GetPixelIndex(x,y)]);
				}
			}
		}
#endif //CXIMAGE_SUPPORT_SELECTION
	} else {
		for(DWORD j=0; j<head.biClrUsed; j++){
			color = GetPaletteColor((BYTE)j);
			color.rgbRed = pLut[color.rgbRed];
			color.rgbGreen = pLut[color.rgbGreen];
			color.rgbBlue = pLut[color.rgbBlue];
			SetPaletteColor((BYTE)j,color);
		}
	}
	return true;

}
#endif
#if 0
bool CxImageEdge(long Ksize)
{

	long 		k2 = Ksize/2;
	long 		kmax= Ksize-k2;
	uint8_t 	r,g,b,rr,gg,bb;
	long		j, k;
	long xmin,xmax,ymin,ymax;
	uint16_t			(*form)[1024];
	uint16_t			(*to)[1024];
	uint16_t		color;
	form 		= (uint16_t(*)[1024])USER_SCREEN2_ADDR;
	to 			= (uint16_t(*)[1024])USER_SCREEN3_ADDR;
	

/*
	if (pSelection){
		xmin = info.rSelectionBox.left; xmax = info.rSelectionBox.right;
		ymin = info.rSelectionBox.bottom; ymax = info.rSelectionBox.top;
	} else {
		xmin = ymin = 0;
		xmax = head.biWidth; ymax=head.biHeight;
	}
*/
	for(long y=ymin; y<ymax; y++)
	{
		for(long x=xmin; x<xmax; x++)
		{
				r=b=g=0;
				rr=bb=gg=255;
				for( j=-k2;j<kmax;j++)
				{
					for( k=-k2;k<kmax;k++)
					{
						//c=GetPixelColor(x+j,y+k);
						color=form[y+k][x+j]
						/*
						if (c.rgbRed > r) r=c.rgbRed;
						if (c.rgbGreen > g) g=c.rgbGreen;
						if (c.rgbBlue > b) b=c.rgbBlue;

						if (c.rgbRed < rr) rr=c.rgbRed;
						if (c.rgbGreen < gg) gg=c.rgbGreen;
						if (c.rgbBlue < bb) bb=c.rgbBlue;
						*/
					}
				}
				/*
				c.rgbRed   = 255-abs(r-rr);
				c.rgbGreen = 255-abs(g-gg);
				c.rgbBlue  = 255-abs(b-bb);
				*/
				to[y][x]=color;
			}
		}
	}
	return true;
}
#endif
void EdgeDetection_Robert(uint16_t dx, uint16_t dy, uint16_t width, uint16_t height, uint32_t addr, uint16_t display)
{
	uint16_t 	centerValue1=0;
	uint16_t 	centerValue2=0;
	uint16_t 	sum=0;
	uint16_t		i = 0;
	uint16_t		j = 0;
	uint16_t		x = 0;
	uint16_t		y = 0;
	uint16_t		row = 0;
	uint16_t		column = 0;		
	int16_t mask1[3][3]={ 	0,0,-1,
										0,1,0,
										0,0,0};
	int16_t mask2[3][3]={	-1,0,0,
										0,1,0,
										0,0,0};

	uint32_t		View_Y = height;
	uint32_t		View_X = width;
	uint16_t			(*form)[1024];
	uint16_t			(*to)[1024];
	form 		= (uint16_t(*)[1024])USER_SCREEN2_ADDR;
	to 			= (uint16_t(*)[1024])USER_SCREEN3_ADDR;

	SetGraphicsPage(LCD_SCREEN);				  
	for( row = View_Y; row > 0; row-- )
	{
		for( column = 0; column < View_X; column++ )
		{
			for(i=0; i<3; i++)
			{
				for(j=0; j<3; j++)
				{
					//centerValue1 += (form[i+column][j+row] & 0x1f) * mask1[i][j];	
					//centerValue2 += (form[i+column][j+row] & 0x1f) * mask2[i][j];
					centerValue1 += form[j+row][i+column]  * mask1[i][j];	
					centerValue2 += form[j+row][i+column]  * mask2[i][j];
				}
			}
			sum = abs(centerValue1) + abs(centerValue2);
			
			if( sum > 32 ) sum = 32;
			to[row+1][column+1]=(unsigned char)sum;
			
			centerValue1=0;
			centerValue2=0;
			sum=0;
		}
	}
    if(display == ON)
	{
		for(y=dy; y<=View_Y; y++) 
		{
			for(x=dx; x<=View_X; x++)
			{
				BmpPlot( x, y,to[y][x]);			
			}
		}
	}
}
void EdgeDetection_Prewitt(uint16_t dx, uint16_t dy, uint16_t width, uint16_t height, uint32_t addr, uint16_t display)
{
	uint16_t 	centerValue1=0;
	uint16_t 	centerValue2=0;
	uint16_t 	sum=0;
	uint16_t		i = 0;
	uint16_t		j = 0;
	uint16_t		x = 0;
	uint16_t		y = 0;
	uint16_t		row = 0;
	uint16_t		column = 0;		
	int mask1[3][3]={ -1,0,1,
		              -1,0,1,
	                  -1,0,1};
	int mask2[3][3]={ 1,1,1,
		              0,0,0,
                    -1,-1,-1};


	uint32_t		View_Y = height;
	uint32_t		View_X = width;
	uint16_t			(*form)[1024];
	uint16_t			(*to)[1024];
	form 		= (uint16_t(*)[1024])USER_SCREEN2_ADDR;
	to 			= (uint16_t(*)[1024])USER_SCREEN3_ADDR;

	SetGraphicsPage(LCD_SCREEN);				  
	for( row = View_Y; row > 0; row-- )
	{
		for( column = 0; column < View_X; column++ )
		{
			for(i=0; i<3; i++)
			{
				for(j=0; j<3; j++)
				{
					//centerValue1 += (form[i+column][j+row] & 0x1f) * mask1[i][j];	
					//centerValue2 += (form[i+column][j+row] & 0x1f) * mask2[i][j];
					centerValue1 += (form[j+row][i+column] & 0x1f) * mask1[i][j];	
					centerValue2 += (form[j+row][i+column] & 0x1f) * mask2[i][j];
				}
			}
			sum = abs(centerValue1) + abs(centerValue2);
			
			if( sum > 255 ) sum = 255;
			to[row+1][column+1]=(unsigned char)sum;
			
			centerValue1=0;
			centerValue2=0;
			sum=0;
		}
	}
    if(display == ON)
	{
		for(y=dy; y<=View_Y; y++) 
		{
			for(x=dx; x<=View_X; x++)
			{
				BmpPlot( x, y,to[y][x]);			
			}
		}
	}
}
void EdgeDetection_Sobel(uint16_t dx, uint16_t dy, uint16_t width, uint16_t height, uint32_t addr, uint16_t display)
{
	uint16_t 	centerValue1=0;
	uint16_t 	centerValue2=0;
	uint16_t 	sum=0;
	uint16_t		i = 0;
	uint16_t		j = 0;
	uint16_t		x = 0;
	uint16_t		y = 0;
	uint16_t		row = 0;
	uint16_t		column = 0;		
	int16_t mask1[3][3]={  -1,0,1,
		               -2,0,2,
	                   -1,0,1	};
	int16_t mask2[3][3]={ 	1,2,1,
		              	0,0,0,
                       -1,-2,-1	};

	uint32_t		View_Y = height;
	uint32_t		View_X = width;
	uint16_t			(*form)[1024];
	uint16_t			(*to)[1024];
	form 		= (uint16_t(*)[1024])USER_SCREEN2_ADDR;
	to 			= (uint16_t(*)[1024])USER_SCREEN3_ADDR;

	SetGraphicsPage(LCD_SCREEN);				  
	for( row = View_Y; row > 0; row-- )
	{
		for( column = 0; column < View_X; column++ )
		{
			for(i=0; i<3; i++)
			{
				for(j=0; j<3; j++)
				{
					//centerValue1 += (form[i+column][j+row] & 0x1f) * mask1[i][j];	
					//centerValue2 += (form[i+column][j+row] & 0x1f) * mask2[i][j];
					centerValue1 += (form[j+row][i+column] & 0x1f) * mask1[i][j];	
					centerValue2 += (form[j+row][i+column] & 0x1f) * mask2[i][j];
				}
			}
			sum = abs(centerValue1) + abs(centerValue2);
			
			if( sum > 255 ) sum = 255;
			to[row+1][column+1]=(unsigned char)sum;
			
			centerValue1=0;
			centerValue2=0;
			sum=0;
		}
	}
    if(display == ON)
	{
		for(y=dy; y<=View_Y; y++) 
		{
			for(x=dx; x<=View_X; x++)
			{
				BmpPlot( x, y,to[y][x]);			
			}
		}
	}
}
void EdgeDetection_Canny(uint16_t dx, uint16_t dy, uint16_t width, uint16_t height, uint32_t addr, uint16_t display)
{
	//uint16_t 	centerValue1=0;
	
	uint32_t		y = 0;
	uint32_t		x = 0;
	uint32_t		c = 0;
	uint32_t		cc = 0;
	//uint16_t 	sum=0;
	//uint16_t		i = 0;
	//uint16_t		j = 0;
	//uint16_t		row = 0;
	//uint16_t		column = 0;		

	uint32_t		View_Y = height;
	uint32_t		View_X = width;
	
/*
	uint16_t mask[5][5]={  2,	4,		5,		4,		2,
									4,	9,		12,	9,		4,
									5,	12,	15,	12,	5,
									4,	9,		12,	9,		4,
									2,	4,		5,		4,		2 };
*/
	uint16_t			(*form)[1024];
	uint16_t			(*to)[1024];
	form 		= (uint16_t(*)[1024])USER_SCREEN3_ADDR;
	to 			= (uint16_t(*)[1024])USER_SCREEN2_ADDR;
	
	SetGraphicsPage(LCD_SCREEN);
		
	for( y = View_Y; y > 0; y-- ) 
    {
    	for( x = 0; x < View_X; x++ )
    	{
    		cc = -form[y-1][x-1] - 1 * form[y][x-1] - form[y-1][x+1];		
    		cc += form[y-1][x+1] + 1 * form[y][x+1] + form[y+1][x+1];
    		c = abs(cc);

    		cc = -form[y-1][x-1] - 1 * form[y-1][x] - form[y-1][x+1];
			
    		cc += form[y+1][x-1] + 1 * form[y+1][x] + form[y+1][x+1];
    		c += abs(cc);
    		to[y][x]= (unsigned char)c;		
       	}
    } 
	
    if(display == ON)
	{
		for(y=dy; y<=View_Y; y++) 
		{
			for(x=dx; x<=View_X; x++)
			{
				BmpPlot( x, y,to[y][x]);			
			}
		}
	}
	//Dprintf("EdgeDetection_Canny \n");
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Perform circle_based transformations.
 * \param type - for different transformations
 * - 0 for normal (proturberant) FishEye
 * - 1 for reverse (concave) FishEye
 * - 2 for Swirle 
 * - 3 for Cilinder mirror
 * - 4 for bathroom
 *
 * \param rmax - effect radius. If 0, the whole image is processed
 * \param Koeff - only for swirle
 * \author Arkadiy Olovyannikov ark(at)msun(dot)ru
 */
uint16_t CxImageCircleTransform(uint16_t dx, uint16_t dy, uint16_t width, uint16_t height, uint32_t addr, uint16_t display, int type,long rmax,float Koeff)
{
	long xmin,xmax,ymin,ymax,xmid,ymid;
	long nx,ny;
	double angle,radius,rnew;
	long y, x;
	
	uint32_t		View_Y = height;
	uint32_t		View_X = width;	
	uint16_t			(*form)[1024];
	uint16_t			(*to)[1024];
	form 		= (uint16_t(*)[1024])USER_SCREEN2_ADDR;
	to 			= (uint16_t(*)[1024])USER_SCREEN3_ADDR;
	
	xmin  = ymin = 0;
	xmax = View_X; 
	ymax =View_Y;
	
	xmid = (long) (View_X/2);
	ymid = (long) (View_Y/2);

	if (!rmax) rmax=(long)sqrt((float)((xmid-xmin)*(xmid-xmin)+(ymid-ymin)*(ymid-ymin)));
	if (Koeff==0.0f) Koeff=1.0f;
/*
	for( row = View_Y; row > 0; row-- )
	{
		for( column = 0; column < View_X; column++ )
*/
	for( y=ymin; y<ymax; y++)
	{
		for( x=xmin; x<xmax; x++)
		{
			nx=xmid-x;
			ny=ymid-y;
			radius=sqrt((float)(nx*nx+ny*ny));
			
			if (radius<rmax) 
			{
				angle=atan2((double)ny,(double)nx);
				if (type==0)	  rnew=radius*radius/rmax;
				else if (type==1) rnew=sqrt(radius*rmax);
				else if (type==2) {rnew=radius;angle += radius / Koeff;}
				if (type<3)
				{
					nx = xmid + (long)(rnew * cos(angle));
					ny = ymid - (long)(rnew * sin(angle));
				}
				else if (type==3)
				{
					nx = (long)fabs((angle*xmax/6.2831852));
					ny = (long)fabs((radius*ymax/rmax));
				}
				else 
				{
					nx=x+(x%32)-16;
					ny=y;
				}
			}
			else 
			{ 
				nx=-1;
				ny=-1;
			}
			to[ny][nx] = form[ny][nx];
		}
	}
    if(display == ON)
	{
		for(y=dy; y<=View_Y; y++) 
		{
			for(x=dx; x<=View_X; x++)
			{
				BmpPlot( x, y,to[y][x]);			
			}
		}
	}
	
	return true;
}


int CopyRectVideo(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height, uint32_t addr)
{
	uint32_t 		x = 0;
	uint32_t			y = 0;
	uint32_t			ScreenPage=addr;
	uint32_t 		SrcAddr = 0;

	uint16_t			(*VRAM_FileBuf)[1024];
	
	VRAM_FileBuf = (uint16_t(*)[1024])USER_SCREEN2_ADDR;
	//Dprintf("USER_SCREEN1_ADDR = %08x \n",*VRAM_FileBuf);
	
	for(y=sy; y<=height; y++) 
	{
		SrcAddr = ScreenPage + (((( sy + y ) * VIDEO_CAMERA_VIEW_X ) + sx ) * 2 );
		for(x=sx; x<=width; x++)
		{
			VRAM_FileBuf[y][x] = *(uint16_t *) (SrcAddr);
			SrcAddr += 2; 
		}		
	}
	return 0; 
}



int CopyRectVideo1(uint32_t page, uint32_t sx, uint32_t sy, uint32_t width, uint32_t height)
{
	uint32_t 		x = 0;
	uint32_t			y = 0;
	uint32_t			ScreenPage=page;
	uint32_t 		SrcAddr = 0;

	uint16_t			(*VRAM_FileBuf)[768];
	
	VRAM_FileBuf = (uint16_t(*)[768])USER_SCREEN2_ADDR;
	Dprintf("USER_SCREEN1_ADDR = %08x \n",*VRAM_FileBuf);
	
	for(y=0; y<=height; y++) 
	{
		SrcAddr = ScreenPage + (((( sy + y ) * VIDEO_CAMERA_VIEW_X ) + sx ) * 2 );
		for(x=0; x<=width; x++)
		{
			VRAM_FileBuf[x][y] = *(uint16_t *) (SrcAddr);
			//BmpPlot( x, y, VRAM_FileBuf[x][y]);			
			SrcAddr += 2; 
		}		
	}
	//Dprintf("CopyRectVideo \n");
	return 0; 
}
void EdgeDetection_Canny1(
										uint32_t 	sx1, 
										uint32_t 	sy1, 
										uint32_t 	display
										)
{
	uint32_t		y = 0;
	uint32_t		x = 0;
	uint32_t		c = 0;
	uint32_t		cc = 0;
	//uint32_t		sx = sx1;
	//uint32_t		sy = sy1;

	uint32_t		View_Y = 768;//VIDEO_CAMERA_VIEW_Y;
	uint32_t		View_X =  1024;//VIDEO_CAMERA_VIEW_X;

	uint16_t			(*form)[1024];
	uint16_t			(*to)[1024];
	form 		= (uint16_t(*)[1024])USER_SCREEN2_ADDR;
	to 			= (uint16_t(*)[1024])USER_SCREEN3_ADDR;
	
	SetGraphicsPage(LCD_SCREEN);
	for( y = View_Y; y > 0; y-- ) 
    {
    	for( x = 0; x < View_X; x++ )
    	{
    		cc = -form[x-1][y-1] - 1 * form[x-1][y] - form[x-1][y+1];		
    		cc += form[x+1][y-1] + 1 * form[x+1][y] + form[x+1][y+1];
    		c = abs(cc);

    		cc = -form[x-1][y-1] - 1 * form[x][y-1] - form[x+1][y-1];
			
    		cc += form[x-1][y+1] + 1 * form[x][y+1] + form[x+1][y+1];
    		c += abs(cc);
    		to[x][y]= (unsigned char)c;		
       	}
    } 
	
    if(display == ON)
	{
		/*
		for(y = View_Y; y > 0; y--)
		{
			for(x = 0; x < View_X; x++)
			{
				BmpPlot( sx+x, sy+y, *( *(to+x)+y));
			}
		}
		*/
		
		for(y=0; y<=View_Y; y++) 
		{
			for(x=0; x<=View_X; x++)
			{
				BmpPlot( x, y,to[y][x]);			
			}
		}
	}
	//Dprintf("EdgeDetection_Canny \n");
}
#endif

