#include "texted.h"

int main()
{
	texted_config editor_conf = inittextedparams("/home/elias/git-repos/eltex/text_editor/file.txt", "", 0, 0);
	//signal(SIGWINCH, resizetexted);
	texted editor = inittexted(editor_conf);
	edittexted(&editor);
	return 0;
}