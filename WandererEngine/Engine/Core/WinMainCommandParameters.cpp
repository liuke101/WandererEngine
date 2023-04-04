#include "WinMainCommandParameters.h"


FWinMainCommandParameters::FWinMainCommandParameters(HINSTANCE InhInstance, HINSTANCE InpreInstance, PSTR IncmdLine, int InshowCmd)
	:HInstance(InhInstance),
	PreInstance(InpreInstance),
	CmdLine(IncmdLine),
	ShowCmd(InshowCmd)
{

}

//test