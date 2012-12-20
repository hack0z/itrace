/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * globals
 */
static tb_gstream_t* 		g_log = TB_NULL;

/* //////////////////////////////////////////////////////////////////////////////////////
 * init
 */
static tb_void_t __attribute__((constructor)) it_init()
{
	tb_abort();
	// init tbox
	tb_init(TB_NULL, 0);

	// init log
	g_log = tb_gstream_init_from_url("/tmp/itrace.log");
	tb_assert_and_check_return(g_log);

	// open log
	tb_gstream_ctrl(g_log, TB_FSTREAM_CMD_SET_FLAGS, TB_FILE_WO | TB_FILE_CREAT | TB_FILE_TRUNC);
	if (!tb_gstream_bopen(g_log)) return ;

	tb_gstream_printf(g_log, "hello itrace: init!\n");
	tb_gstream_bfwrit(g_log, TB_NULL, 0);
}
