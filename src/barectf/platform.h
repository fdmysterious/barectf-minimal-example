/**
 * Test platform to implement barectf on a rp pico
 * \author Florian Dupeyron <florian.dupeyron@mugcat.fr>
 * \date May 2023
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "barectf.h"

/* --------------------- Data types */

/**
 * Return code for pico trace module
 */
enum Pico_Trace_Return_Code {
	/** Default code indicating unexpected behaviour */
	PICO_TRACE_UNKNOWN,

	/** Everything is OK */
	PICO_TRACE_OK,

	/** Something wrong happened */
	PICO_TRACE_FAIL,
};


/** Structure holding platform context */
struct Pico_Trace_Platform_Ctx;


/* --------------------- Public interface */

/**
 * Inits the platform
 * \param buf size Size of input buffer?
 * \param data_stream_file_path path to file stream
 * \return context data
 */
enum Pico_Trace_Return_Code pico_trace_init(void);


/**
 * Platform finalization
 * \param ctx pointer to platform context
 */
enum Pico_Trace_Return_Code pico_trace_end(void);


/**
 * Context pointer access function
 * \param ctx pointer to platform context
 * \return pointer to barectf context
 */
struct barectf_default_ctx* pico_trace_barectf_ctx_get(void);

#ifdef __cplusplus
}
#endif
