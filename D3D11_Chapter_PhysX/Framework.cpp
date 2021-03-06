#include "Framework.h"

Framework::Framework() :
			m_wnd(nullptr),
			m_render(nullptr),
			m_input(nullptr),
			m_physics(nullptr),
			m_stepper(nullptr),
			m_textManager(nullptr),
			m_fps(nullptr),
			m_objectManager(nullptr),
			m_newInput(nullptr),
			m_init(false),
			m_isRunning(false) {
				
}

Framework::~Framework() {

}

//��� ������ � �������� ��������� ����� ����� ������� ���������� ������ InputListener, � ������� ��������������
//������, ������������ ������� ���������� �� �����-���� �������� ������������
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
	m_objectManager = new ObjectManager();
	m_newInput = new Input();

	m_fps->init();

	if(!m_wnd || !m_input) {
		Log::get()->err("�� ������� �������� ������");
		return false;
	}

	//������� �������� �������� �� ���������
	DescWindow desc;
	if(!m_wnd->create(desc)) {
		Log::get()->err("�� ������� ������� ����");
		return false;
	}
	m_wnd->setInputMgr(m_input);

	if(!m_newInput->init(m_wnd->getHInstance(), m_wnd->getHWND())) {
		Log::get()->err("�� ������� ������� Input");
		return false;
	}

	if(!m_render->createDevice(m_wnd->getHWND())) {
		Log::get()->err("�� ������� ������� ����������");
		return false;
	}


	if(!m_physics->init(m_render)) {
		Log::get()->err("�� ������� ������� PhysX");
		return false;
	}

	if(!m_scene->initScene(m_physics)) {
		Log::get()->err("�� ������� ������� Stepper");
		return false;
	}

	if(!m_stepper->init(m_scene)) {
		Log::get()->err("�� ������� ������� Stepper");
		return false;
	}

	if(!m_scene->createPlane()) {
		Log::get()->err("�� ������� ������� Plane");
		return false;
	}

	m_textManager = new TextManager(m_render);
	if(!m_textManager->addFont("key_fps_caption", "Res\\2.fnt")) {
		Log::get()->err("Font Create Failed");
		return false;
	}
	//CameraInputListener* pCameraInputListener = new CameraInputListener(m_render->getCamera());
	//m_input->addListener(pCameraInputListener);
	
	//Temporary use of ObjectManager
	if(!m_objectManager->addBox("box1", m_physics, m_physics->getPxPhysics()->createMaterial(0.5f, 0.5f, 0.1f),
								PxVec3(-3.0f, 8.0f, 0.0f), 0.8f, PxVec3(0.0f, 0.0f, 0.0f), PxVec3(2.0f, 0.5f, 1.0f))) {
		return false;
	}
	if(!m_objectManager->addBox("box2", m_physics, m_physics->getPxPhysics()->createMaterial(0.5f, 0.5f, 0.1f),
								PxVec3(-3.0f, 0.0f, 0.0f), 0.8f, PxVec3(0.0f, 0.0f, 0.0f), PxVec3(4.0f, 4.0f, 4.0f))) {
		return false;
	}
	if(!m_objectManager->addBox("box3", m_physics, m_physics->getPxPhysics()->createMaterial(0.5f, 0.5f, 0.1f),
								PxVec3(3.0f, 8.0f, 0.0f), 0.8f, PxVec3(0.0f, 0.0f, 0.0f), PxVec3(4.0f, 4.0f, 4.0f))) {
		return false;
	}
	if(!m_objectManager->addBox("box4", m_physics, m_physics->getPxPhysics()->createMaterial(0.5f, 0.5f, 0.1f),
								PxVec3(3.0f, 6.0f, 0.0f), 0.8f, PxVec3(0.0f, 0.0f, 0.0f), PxVec3(2.0f, 0.5f, 1.0f))) {
		return false;
	}
	
	m_objectManager->engageObject("box1");
	m_objectManager->engageObject("box2");
	m_objectManager->engageObject("box3");
	m_objectManager->engageObject("box4");

	if(!m_render->initObjects(m_objectManager)) {
		Log::get()->err("�� ������� ���������������� ������� � �������");
		return false;
	}

	m_scene->addActors(m_objectManager);

	m_textManager->addStaticText("key_fps_text", "key_fps_caption", L"FPS:", 1.0f, 1.0f, 0.0f, 10.0f, 10.0f);
	m_textManager->addDynamicText("key_fps_count", "key_fps_caption", L"0", 1.0f, 1.0f, 0.0f, 80.0f, 10.0f, 8);

	m_init = true;
	return true;
}

void Framework::run() {
	if(m_init) {
		m_isRunning = true;

		Timer::startTimer();
		double frameTime;
		//	�������-������� ��� ����������� ������ ������� �������
		frameTime = Timer::getFrameTime();

		while(m_isRunning) {
			//����������, ������
			frameTime = Timer::getFrameTime();
			m_isRunning = m_frame(frameTime);
		}
	}
}

bool Framework::m_frame(double dt) {
	//������������ ������� ����
	m_wnd->runEvent();
	//���� ���� ��������� - ��������� ����
	if(!m_wnd->isActive()) {
		//return true;
	}

	//���� ���� ���� ������� - ��������� ������ ������
	if(m_wnd->isExit()) {
		return false;
	}

	//���� ���� �������� ������
	if(m_wnd->isResize()) {

	}

	//	TODO REFACTOR
	m_newInput->detectInput(dt, m_render->getCamera());

	m_fps->frame();				//�������� ����� ����� � Fps
		
	preRender(dt);				//��������� ���������� �����

	m_objectManager->updateObjects();	//��������� ������� ��������

	m_textManager->setDynamicText("key_fps_count", m_fps->getFpsStr());		//������ ����� Fps � ������� ������������� ������

	m_textManager->drawText();	//� ������� ��� �� �����

	if(!m_render->draw(dt)) {	//������ ����� ���������� ��� ��������� � ������ ��������� ����� ���������
		return false;
	}

	postRender(dt);		//������ ������� ������ � �������� ����� - ������ ��� ������������ �������� ���������� �� ������
	
	return true;		//true ��������, ��� �� ��������� �������� ����� ���������� ���� �����
}

void Framework::preRender(double dt) {
	m_stepper->advance(dt);					//����������, ���� ������� ������� �� ���������
	m_scene->getPxScene()->fetchResults(true);	//������ ����� �������� �����

	m_render->beginFrame(dt);					//������ ����� DirectX ����������� ������� ������� ����
}

void Framework::postRender(double dt) {
	m_render->endFrame();
}

void Framework::Close() {
	m_init = false;
	m_render->shutdown();
	m_objectManager->destroy();
	_DELETE(m_render);
	_CLOSE(m_wnd);
	_CLOSE(m_input);
	_CLOSE(m_scene);
	_CLOSE(m_stepper);
	_CLOSE(m_textManager);
	_CLOSE(m_physics);
	system("PAUSE");
}

