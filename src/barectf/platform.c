/**
 * Test platform to implement barectf on a rp pico
 * \author Florian Dupeyron <florian.dupeyron@mugcat.fr>
 * \date May 2023
 */

#include "platform.h"
#include "pico/stdio.h"
#include "pico/time.h"

#include <stdint.h>

#define BUF_SIZE 256
#define STDIO_HANDLE_STDOUT (0x1U)

/* --------------------- Macros */
#ifdef __cplusplus
# define _FROM_VOID_PTR(_type, _value)	static_cast<_type *>(_value)
#else
# define _FROM_VOID_PTR(_type, _value)	((_type *) (_value))
#endif

/* --------------------- Forward declarations */

int _write(int fd, char* data, size_t len);


/* --------------------- Data types */

struct Pico_Trace_Platform_Ctx {
	struct barectf_default_ctx barectf_ctx;
	
	uint8_t buf[BUF_SIZE];
};


/* --------------------- Accessors */

struct Pico_Trace_Platform_Ctx* pico_trace_private_instance_get(void)
{
	static struct Pico_Trace_Platform_Ctx ctx;
	return &ctx;
}


/* --------------------- Private interface */

/**
 * Return the current clock in nanosecs
 */
static uint64_t pico_trace_private_clock_get(void* const data)
{
	struct Pico_Trace_Platform_Ctx *platform = _FROM_VOID_PTR(struct Pico_Trace_Platform_Ctx, data);
	(void)platform; /* Unused in this function */

	absolute_time_t now = get_absolute_time();
	uint64_t now_us = to_us_since_boot(now);

	return now_us * 1000U;
}

/**
 * Opens a packet =
 */
static void pico_trace_private_packet_open(void* const data)
{
	struct Pico_Trace_Platform_Ctx *platform = _FROM_VOID_PTR(struct Pico_Trace_Platform_Ctx, data);
	barectf_default_open_packet(&platform->barectf_ctx);
}

/**
 * Closes the packet
 */
static void pico_trace_private_packet_close(void* const data)
{
	struct Pico_Trace_Platform_Ctx *platform = _FROM_VOID_PTR(struct Pico_Trace_Platform_Ctx, data);
	barectf_default_close_packet(&platform->barectf_ctx);

	/* Write the packet */
	_write(STDIO_HANDLE_STDOUT,
		(char*)barectf_packet_buf(&platform->barectf_ctx),
		barectf_packet_buf_size(&platform->barectf_ctx)
	);
	stdio_flush();
}

static int pico_trace_private_is_backend_full(void* const data)
{
	struct Pico_Trace_Platform_Ctx *platform = (struct Pico_Trace_Platform_Ctx*)data;
	(void)platform;
	// TODO //
	return 0;
}


/* --------------------- Public interface */

enum Pico_Trace_Return_Code pico_trace_init(void)
{
	struct Pico_Trace_Platform_Ctx* platform = pico_trace_private_instance_get();

	/* Barectf init */
	struct barectf_platform_callbacks clbk = {
		.default_clock_get_value = pico_trace_private_clock_get,
		.is_backend_full         = pico_trace_private_is_backend_full,
		.open_packet             = pico_trace_private_packet_open,
		.close_packet            = pico_trace_private_packet_close,
	};

	/* Init pico stdio */
	stdio_init_all();

	// TODO // Remove
	stdio_flush();

	/* Init barectf */
	barectf_init(&platform->barectf_ctx, platform->buf, BUF_SIZE, clbk, platform);

	return PICO_TRACE_OK;
}


enum Pico_Trace_Return_Code pico_trace_end(void)
{
	struct Pico_Trace_Platform_Ctx* platform = pico_trace_private_instance_get();

	if(
		    barectf_packet_is_open (&platform->barectf_ctx)
		&& !barectf_packet_is_empty(&platform->barectf_ctx)
	) {
		/* Close the packet */
		pico_trace_private_packet_close(&platform->barectf_ctx);
	}

	return PICO_TRACE_OK;
}


struct barectf_default_ctx * pico_trace_barectf_ctx_get(void)
{
	struct Pico_Trace_Platform_Ctx* platform = pico_trace_private_instance_get();
	return &platform->barectf_ctx;
}
