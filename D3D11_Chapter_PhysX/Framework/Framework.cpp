#include "Framework.h"

Framework::Framework() :
			m_wnd(nullptr),
			m_render(nullptr),
			m_input(nullptr),
			m_physics(nullptr),
			m_stepper(nullptr),
			m_textManager(nullptr),
			m_fps(nullptr),
			m_init(false),
			m_isRunning(false) {
				
}

Framework::~Framework() {

}

//Для работы с системой обработки ввода нужно создать наследника класса InputListener, в котором переопределить
//методы, определяющие реакцию приложения на какие-либо действия пользователя
void Framework::addInputListener(InputListener* listener) {
	if(m_input) {
		m_input->addListener(listener);
	}
}

	

bool Framework::init() {
	m_wnd = new Window();
	m_input = new InputMgr();
	m_log = new Log();
	m_physics = new Physics();
	m_scene = new Scene();
	m_stepper = new Stepper();
	m_fps = new Fps();

	m_fps->init();
	startTime = GetTickCount();

	if(!m_wnd || !m_input) {
		Log::get()->err("Не удалось выделить память");
		return false;
	}

	//Создаем значения настроек по умолчанию
	DescWindow desc;
	if(!m_wnd->create(desc)) {
		Log::get()->err("Не удалось создать окно");
		return false;
	}
	m_wnd->setInputMgr(m_input);

	if(!m_render->createDevice(m_wnd->getHWND())) {
		Log::get()->err("Не удалось создать рендер");
		return false;
	}

	if(!m_physics->init(m_render)) {
		Log::get()->err("Не удалось создать PhysX");
		return false;
	}

	if(!m_scene->initScene(m_physics)) {
		Log::get()->err("Не удалось создать Stepper");
		return false;
	}

	if(!m_stepper->init(m_scene)) {
		Log::get()->err("Не удалось создать Stepper");
		return false;
	}

	if(!m_scene->createPlane()) {
		Log::get()->err("Не удалось создать Plane");
		return false;
	}

	if(!m_scene->createActors()) {
		Log::get()->err("Не удалось создать актеров");
		return false;
	}

	m_scene->addActors();

	m_textManager = new TextManager(m_render);
	if(!m_textManager->addFont("key_fps_caption", "Res\\2.fnt")) {
		Log::get()->err("Font Create Failed");
		return false;
	}

	m_textManager->addStaticText("key_fps_text", "key_fps_caption", L"FPS:", 1.0f, 1.0f, 0.0f, 10.0f, 10.0f);
	m_textManager->addDynamicText("key_fps_count", "key_fps_caption", L"0", 1.0f, 1.0f, 0.0f, 80.0f, 10.0f, 8);

	m_init = true;
	return true;
}

void Framework::run() {
	if(m_init) {
		m_isRunning = true;

		float currentFrameTime = startTime;
		float prevFrameTime;
		float dt;
			
		while(m_isRunning) {
			//Считаем пройденное время с предыдущей итерации
			prevFrameTime = currentFrameTime;
			currentFrameTime = GetTickCount();
			dt = currentFrameTime - prevFrameTime;
			dt /= 1000;					//С секундами работать проще

			//Симулируем, рисуем
			m_isRunning = m_frame(dt);
		}
	}
}

bool Framework::m_frame(float dt) {
	//Обрабатываем события окна
	m_wnd->runEvent();
	//Если окно неактивно - завершаем кадр
	if(!m_wnd->isActive()) {
		return true;
	}

	//Если окно было закрыто - завершаем работу движка
	if(m_wnd->isExit()) {
		return false;
	}

	//Если окно изменило размер
	if(m_wnd->isResize()) {

	}

	m_fps->frame();				//Замеряем время кадра в Fps
		
	preRender(dt);				//Симуляция происходит здесь

	//m_scene->updateActors()
	m_textManager->setDynamicText("key_fps_count", m_fps->getFpsStr());		//Меняем число Fps в объекте динамического текста

	m_textManager->drawText();	//И выводим его на экран

	if(!m_render->draw(dt)) {	//Теперь можно отрисовать все остальное с учетом изменений после симуляции
		return false;
	}

	postRender(dt);		//Меняем местами задний и передний буфер - только что отрисованная картинка появляется на экране
	
	return true;		//true означает, что на следующей итерации снова запустится этот метод
}

void Framework::preRender(float dt) {
	m_stepper->advance(dt);						//Собственно, даем команду физиксу на симуляцию
	m_scene->getPxScene()->fetchResults(true);	//Теперь можно обновить сцену

	m_render->beginFrame(dt);					//Обычно здесь DirectX закрашивает рабочую область окна
}

void Framework::postRender(float dt) {
	m_render->endFrame();
}

void Framework::Close() {
	m_init = false;
	m_render->shutdown();
	_DELETE(m_render);
	_CLOSE(m_wnd);
	_CLOSE(m_input);
	_CLOSE(m_scene);
	_CLOSE(m_stepper);
	_CLOSE(m_textManager);
	_CLOSE(m_physics);
	system("PAUSE");
}

