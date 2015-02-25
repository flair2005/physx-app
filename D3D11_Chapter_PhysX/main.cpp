#include "Framework\Framework.h"

int main() {
	Framework framework;
	Render* render = new Render();

	framework.setRender(render);
	framework.init();

	framework.run();

	framework.Close();

	return 0;
}