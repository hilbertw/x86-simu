#include <stdio.h>

void	test_add8();
void	test_add16();
void	test_add32();
void	test_xor8();
void	test_xor16();
void	test_xor32();
void	test_or8();
void	test_or16();
void	test_or32();
void	test_and8();
void	test_and16();
void	test_and32();
void	test_sub8();
void	test_sub16();
void	test_sub32();
void	test_adc8();
void	test_adc16();
void	test_adc32();
void	test_subb8();
void	test_subb16();
void	test_subb32();
void		test_shr18();
void		test_shr116();
void		test_shr132();
void		test_sar18();
void		test_sar116();
void		test_sar132();
void		test_shl18();
void		test_shl116();
void		test_shl132();
void		test_ror18();
void		test_ror116();
void		test_ror132();
void		test_rol18();
void		test_rol116();
void		test_rol132();
void		test_rcr18();
void		test_rcr116();
void		test_rcr132();
void		test_rcl18();
void		test_rcl116();
void		test_rcl132();
void		test_shrn8();
void		test_shrn16();
void		test_shrn32();
void		test_sarn8();
void		test_sarn16();
void		test_sarn32();
void		test_shln8();
void		test_shln16();
void		test_shln32();
void		test_rorn8();
void		test_rorn16();
void		test_rorn32();
void		test_roln8();
void		test_roln16();
void		test_roln32();
void		test_rcrn8();
void		test_rcrn16();
void		test_rcrn32();
void		test_rcln8();
void		test_rcln16();
void		test_rcln32();
void	test_inc8();
void	test_inc16();
void	test_inc32();
void	test_dec8();
void	test_dec16();
void	test_dec32();
void	test_neg8();
void	test_neg16();
void	test_neg32();
void	test_not8();
void	test_not16();
void	test_not32();
int main(int argc, char* argv[])
{
	if(argc>1)
		
	switch( argv[1][0])
	{
	case '8':
	test_sub8();
	test_add8();
	test_adc8();
	test_subb8();
	test_and8();
	test_or8();
	test_xor8();
	test_shr18();
	test_sar18();
	test_ror18();
	test_shl18();
	test_rol18();
	test_rcr18();
	test_shrn8();
	test_rcl18();
	test_sarn8();
	test_shln8();
	test_rorn8();
	test_roln8();
	test_rcrn8();
	test_rcln8();
	test_neg8();
	test_not8();
	test_dec8();
	test_inc8();
	break;

	case 'h':
	test_neg16();
	test_not16();
	test_dec16();
	test_inc16();
	test_subb16();
	test_adc16();
	test_add16();
	test_sub16();
	test_or16();
	test_xor16();
	test_and16();
	test_shr116();
	test_sar116();
	test_ror116();
	test_shl116();
	test_rol116();
	test_rcr116();
	test_rcl116();
	test_shrn16();
	test_sarn16();
	test_shln16();
	test_rorn16();
	test_roln16();
	test_rcrn16();
	test_rcln16();

	break;

	case  '3':
	test_sar132();
	test_shr132();
	test_shl132();
	test_ror132();
	test_rol132();
	test_rcl132();
	test_rcr132();
	test_shrn32();
	test_sarn32();
	test_shln32();
	test_rorn32();
	test_roln32();
	test_rcrn32();
	test_rcln32();
	break;
	case 'n':
	test_neg32();
	test_not32();
	break;
	case '1':
	test_dec32();
	test_inc32();
	break;
	case '+':
	test_add32();
	break;
	case 's':
	test_sub32();
	break;
	case 'o':

	test_or32();
	break;
	case 'x':
	test_xor32();
	break;

	case 'a':
	test_and32();
	break;
	case 'c':
	test_adc32();
	test_subb32();
	break;
	}



	return 0;
}

