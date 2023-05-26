#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/time.h"

#include "barectf/platform.h"


/* -------------- main program */

int main()
{
	const uint LED_PIN = 15;

	pico_trace_init();
	barectf_enable_tracing(pico_trace_barectf_ctx_get(), 1);

	gpio_init(LED_PIN);
	gpio_set_dir(LED_PIN, GPIO_OUT);

	uint32_t c = 0;

    while (true) {
		c++;
		barectf_trace_simple_uint32(pico_trace_barectf_ctx_get(), c);


		gpio_put(LED_PIN, 1);
		sleep_ms(100);
		gpio_put(LED_PIN, 0);
		sleep_ms(100);
	}

	return 0;
}
