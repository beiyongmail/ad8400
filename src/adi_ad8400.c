#include <rtthread.h>
#include <rtdevice.h>

#ifdef PKG_USING_AD8400


#ifdef BSP_AD8400_CS_PIN
#define AD8400_CS_PIN 				BSP_AD8400_CS_PIN
#else
#error No define BSP_AD8400_CS_PIN 
#endif

#ifdef BSP_AD8400_SCK_PIN
#define AD8400_SCK_PIN				BSP_AD8400_SCK_PIN
#else 
#error No define BSP_AD8400_SCK_PIN 	
#endif

#ifdef BSP_AD8400_SI_PIN
#define AD8400_SI_PIN				BSP_AD8400_SI_PIN
#else 
#error No define BSP_AD8400_SI_PIN 	
#endif

#define MCP_CS_SET()				rt_pin_write(AD8400_CS_PIN, PIN_HIGH)
#define MCP_CS_CLC()				rt_pin_write(AD8400_CS_PIN, PIN_LOW)

#define MCP_SCK_SET()				rt_pin_write(AD8400_SCK_PIN, PIN_HIGH)
#define MCP_SCK_CLC()				rt_pin_write(AD8400_SCK_PIN, PIN_LOW)

#define MCP_SI_SET()				rt_pin_write(AD8400_SI_PIN, PIN_HIGH)
#define MCP_SI_CLC()				rt_pin_write(AD8400_SI_PIN, PIN_LOW)

#define GET_BIT(DATA, N)        (((DATA)>>(N)) & 0x0001)

void ad8400_write(uint8_t addr, uint8_t value)
{
	uint16_t send_value = (addr<<8) + value;
	MCP_CS_CLC();
	for(int i=0; i<10; i++) {
		MCP_SCK_CLC();
		if(GET_BIT(send_value, 9-i)) {
			MCP_SI_SET();
		} else {
			MCP_SI_CLC();
		}
		MCP_SCK_SET();
	}
	MCP_CS_SET();
}


static void ad8400_set_value(int argc, char**argv)
{
	if (argc < 3) {
		rt_kprintf("Please input parm!\n");
		return;
	}
	uint32_t addr = atoi(argv[1]);
	if (addr > 3) {
		rt_kprintf("Please input value range : 0~ 3!\n");
		return;
	}

	uint32_t value = atoi(argv[2]);
	if (value > 255) {
		rt_kprintf("Please input value range : 0~ 255!\n");
		return;
	}

	ad8400_write(addr, value);
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(ad8400_set_value, ad8400 set value);

int rt_hw_ad8400_init(void)
{
	rt_pin_mode(AD8400_CS_PIN, PIN_MODE_OUTPUT);
	rt_pin_mode(AD8400_SCK_PIN, PIN_MODE_OUTPUT);
	rt_pin_mode(AD8400_SI_PIN, PIN_MODE_OUTPUT);

	rt_pin_write(AD8400_CS_PIN, PIN_LOW);
	rt_pin_write(AD8400_SCK_PIN, PIN_LOW);
	rt_pin_write(AD8400_SI_PIN, PIN_LOW);
    return 0;
}
INIT_DEVICE_EXPORT(rt_hw_ad8400_init);
#endif


