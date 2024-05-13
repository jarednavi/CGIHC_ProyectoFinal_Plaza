//------------------------------------------------------------------//
//                             EQUIPO 7                             //
//                          PROYECTO FINAL                          //
//                      ****PLAZA COMERCIAL****                     //
//------------------------------------------------------------------//

#include <iostream>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "SOIL2/SOIL2.h"
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"
#include "modelAnim.h"

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();
//void animacion();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

//CAMARA-------------------------------------------
Camera  camera(glm::vec3(27.0f, 19.0f, 37.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
float range = 0.0f;
float rot = 0.0f;
float movCamera = 0.0f;

//variables Starbucks ------------------------------------------------------

//Variables Animacion las cuales son usadas para
//los movimientos de rotacion, traslacion y activacion 
//de animaciones y sentindos
float abrir = 0;

bool sentido = false;
bool anim = false;


float abrirDos = 0;

bool sentidoDos = false;
bool animDos = false;

float vibrar = 0;

bool sentidoTres = false;
bool animTres = false;

float x = 0, y = 0, z = 0;
bool animT = false;
float TazaR = 0;
float TazaRY = 0;

float TarroR = 0;
bool animCuatro = false;

float xT = 0, yT = 0, zT = 0;

float xGU = 0, yGU, zGU = 0, rGU = 0;
float xGD = 0, yGD, zGD = 0, rGD = 0;
float xGT = 0, yGT, zGT = 0, rGT = 0;
float xGC = 0, yGC, zGC = 0, rGC = 0;
float xGCI = 0, yGCI, zGCI = 0, rGCI = 0;
float xGS = 0, yGS, zGS = 0, rGS = 0;

float angLlaveFreg = 0;
bool animCinco = false;
bool movLlaveFreg = true;

//// Keyframes
//
//#define MAX_FRAMES 9
//int i_max_steps = 190;
//int i_curr_steps = 0;
//typedef struct _frame
//{
//	//Variables para GUARDAR Key Frames
//	float posX;		//Variable para PosicionX
//	float posY;		//Variable para PosicionY
//	float posZ;		//Variable para PosicionZ
//	float incX;		//Variable para IncrementoX
//	float incY;		//Variable para IncrementoY
//	float incZ;		//Variable para IncrementoZ
//
//}FRAME;
//
//FRAME KeyFrame[MAX_FRAMES];
//int FrameIndex = 0;			//introducir datos
//bool play = false;
//int playIndex = 0;


// Light attributes
bool active;
float desPuerta = 0.0f;
bool abrirDoor = false;
float movEsfera = 0.0f;
bool usarFuerza = false;
//glm::vec3 PosIni(-95.0f, 1.0f, -45.0f);


//VECTORES DE LUCES--------------------------
glm::vec3 spotLightPos(0.0f, 3.0f, 0.0f);
glm::vec3 spotLightDir(0.0f, -20.0f, 0.0f);


// Deltatime
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(1.0f,6.85f,3.0f),
	glm::vec3(18.3f,5.6f,0.7f),
	glm::vec3(18.3f,5.6f,4.9f),
	glm::vec3(18.5f,3.7f,0.7f),
	//glm::vec3(posX, posY, posZ)

};

glm::vec3 LightP1;
glm::vec3 Light1;
glm::vec3 Light2;
glm::vec3 Light3;
glm::vec3 Light4;

//MAIN PRINCIPAL--------------------------------------------------------------------------
int main(){
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/
	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "****PLAZA COMERCIAL****", nullptr, nullptr);
	if (nullptr == window){
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	printf("%f", glfwGetTime());
	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit()){
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}
	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	// OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//CARGAR SHADERS-----------------------------------------------------------
	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");
	Shader SkyBoxshader("Shaders/SkyBox.vs", "Shaders/SkyBox.frag");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.frag");
	
	//**********************                           ************************
	//**********************C A R G A R   M O D E L O S************************
	//**********************                           ************************
	
	//------------------------------------------------------------------FACHADA
	Model Arbolsolo((char*)"Models/nuplaza/arbol.obj");
	Model Arbolesatras((char*)"Models/nuplaza/arbolesback.obj");
	Model Arbolesizquierda((char*)"Models/nuplaza/arbolesleft.obj");
	Model Autosamarillo((char*)"Models/nuplaza/autosamarillos.obj");
	Model Autosazul((char*)"Models/nuplaza/autosazules.obj");
	Model Autosrojo((char*)"Models/nuplaza/autosrojos.obj");
	Model Bancasatras((char*)"Models/nuplaza/bancasback.obj");
	Model Bardaperi((char*)"Models/nuplaza/barda.obj");
	Model Casetasent((char*)"Models/nuplaza/casetas.obj");
	Model Escalaman((char*)"Models/nuplaza/escalahombre.obj");
	Model Escalawoman((char*)"Models/nuplaza/escalamujer.obj");
	Model Escalagirl((char*)"Models/nuplaza/escalanina.obj");
	Model Escalaboy((char*)"Models/nuplaza/escalanino.obj");
	Model Escalasatras((char*)"Models/nuplaza/escalasback.obj");
	Model Escalasfrente((char*)"Models/nuplaza/escalasfront.obj");
	Model Escalasizquierda((char*)"Models/nuplaza/escalasleft.obj");
	Model Zeus((char*)"Models/nuplaza/escultura.obj");
	Model Espectacularplaza((char*)"Models/nuplaza/espectacular.obj");
	Model Farolluz((char*)"Models/nuplaza/farol.obj");
	Model Fuenteagua((char*)"Models/nuplaza/fuente.obj");
	Model Pisoasfalto((char*)"Models/nuplaza/pisoexterior.obj");
	Model Plantillapiso((char*)"Models/nuplaza/plantilla.obj");
	Model Plumasparking((char*)"Models/nuplaza/plumas.obj");
	Model Rampaparking((char*)"Models/nuplaza/rampa.obj");
	Model entrada((char*)"Models/entrada/entrada.obj");
	
	//----------------------------------------------------------------STAR WARS
	Model Piso((char*)"Models/nufinal/Piso.obj");
	Model Antenaoriginal((char*)"Models/nufinal/antenaor.obj");
	Model Antenasfuera((char*)"Models/nufinal/antenasout.obj");
	Model Antenaarriba((char*)"Models/nufinal/antenaup.obj");
	Model Casamando((char*)"Models/nufinal/casadin.obj");
	//Model Arcoderecho((char*)"Models/nufinal/arcoright.obj");
	Model Barrildentro((char*)"Models/nufinal/barrilin.obj");
	Model Barrilfuera((char*)"Models/nufinal/barrilout.obj");
	Model Cascosfuera((char*)"Models/nufinal/cascos.obj");
	//Model Columnasizquierda((char*)"Models/nufinal/columnasleft.obj");
	//Model Columnasderecha((char*)"Models/nufinal/columnasright.obj");
	Model Domoarriba((char*)"Models/nufinal/domo.obj");
	Model Estantefuera((char*)"Models/nufinal/estante.obj");
	Model Lamparadentro((char*)"Models/nufinal/lamparain.obj");
	Model Lamparasfuera((char*)"Models/nufinal/lamparasout.obj");
	Model Lonaafuera((char*)"Models/nufinal/lona.obj");
	Model Losaarriba((char*)"Models/nufinal/losa.obj");
	Model Mesadentro((char*)"Models/nufinal/mesa.obj");
	Model Muroatras((char*)"Models/nufinal/muroback.obj");
	//Model Murofrenteizquierdo((char*)"Models/nufinal/murofrontleft.obj");
	//Model Murofrentederecho((char*)"Models/nufinal/murofrontright.obj");
	//Model Muroizquierdo((char*)"Models/nufinal/muroleft.obj");
	//Model Muroderecho((char*)"Models/nufinal/muroright.obj");
	Model Naverazor((char*)"Models/nufinal/nave.obj");
	Model Panelpuertafrente((char*)"Models/nufinal/panelpuerta.obj");
	Model Panelverdeatras((char*)"Models/nufinal/panelverde.obj");
	Model Piedrasfuera((char*)"Models/nufinal/piedras.obj");
	Model Pisofrente((char*)"Models/nufinal/pisofront.obj");
	Model Pisodentro((char*)"Models/nufinal/pisoin.obj");
	Model Pisofuera((char*)"Models/nufinal/pisoout.obj");
	Model Plantasadorno((char*)"Models/nufinal/plantas.obj");
	Model Rielesfuera((char*)"Models/nufinal/rieles.obj");
	Model Sillafuera((char*)"Models/nufinal/sillaout.obj");
	Model Sillasdentro((char*)"Models/nufinal/sillasin.obj");
	Model Sofadentro((char*)"Models/nufinal/sofa.obj");
	Model Tazadentro((char*)"Models/nufinal/taza.obj");
	Model Tuboscurvosfuera((char*)"Models/nufinal/tuboscurvos.obj");
	Model Ventanaizquierda((char*)"Models/nufinal/ventanaleft.obj");
	Model Ventanaderecha((char*)"Models/nufinal/ventanaright.obj");
	Model Voladoarriba((char*)"Models/nufinal/volado.obj");
	Model Grogufuera((char*)"Models/nufinal/grogu.obj");
	Model Esferagrogu((char*)"Models/nufinal/esfera.obj");
	Model Puertafrente((char*)"Models/nufinal/puerta.obj");
	Model Ranafuera((char*)"Models/nufinal/rana.obj");

	//----------------------------------------------------------------STARBUCKS

	Model PisoStarbucks((char*)"Models/Modelos/Fachada_Menu/Piso2.obj");
	Model Fachada((char*)"Models/Modelos/Fachada/FachadaFinal.obj");
	Model Banca((char*)"Models/Modelos/BancaPrueba/BancaPrueba.obj");

	Model Menu((char*)"Models/Modelos/Fachada/Menu.obj");
	//Model FocosMenu((char*)"Models/Modelos/Fachada/FocosMenu.obj");

	Model PlantaUno((char*)"Models/Modelos/Plantas/PlantaUno.obj");
	//Model PlantaDos((char*)"Models/Modelos/Plantas/PlantaDos.obj");
	Model PlantaTres((char*)"Models/Modelos/Plantas/PlantaTres.obj");

	Model CajaR((char*)"Models/Starbucks/Barras/CajaR.obj");
	Model Vitrina((char*)"Models/Starbucks/VitrinaDos/Vitrina.obj");
	Model Vidrio((char*)"Models/Starbucks/VitrinaDos/Vidrios.obj");
	Model puertas((char*)"Models/Starbucks/Fachada_Menu/puertas.obj");
	Model puertaUno((char*)"Models/Starbucks/Fachada_Menu/puertaUno.obj");
	Model puertaDos((char*)"Models/Starbucks/Fachada_Menu/puertados.obj");
	Model puertaTres((char*)"Models/Starbucks/Fachada_Menu/puertaTres.obj");
	Model puertaCuatro((char*)"Models/Starbucks/Fachada_Menu/puertaCuatro.obj");
	Model cafetera((char*)"Models/Starbucks/Cafeteras/CafeteraAnim.obj");
	Model CafeteraDos((char*)"Models/Starbucks/Cafeteras/Cafeteras.obj");
	Model Taza((char*)"Models/Starbucks/Cafeteras/Taza.obj");
	Model BarraPrin((char*)"Models/Starbucks/Barras/BarraPrincipal.obj");
	Model BarraTrastes((char*)"Models/Starbucks/Barras/BarraTrastes.obj");
	Model BarraFregadero((char*)"Models/Starbucks/Barras/BarraFreg.obj");
	Model LlaveFregadero((char*)"Models/Starbucks/Barras/LlaveFreg.obj");
	Model ManijaFregadero((char*)"Models/Starbucks/Barras/ManijaFreg.obj");
	Model BarraVasos((char*)"Models/Starbucks/Barras/BarraVasos.obj");
	Model VasoStarUno((char*)"Models/Starbucks/Barras/VasoStarUno.obj");
	Model VasoStarDos((char*)"Models/Starbucks/Barras/VasoStarDos.obj");
	Model VasoStarTres((char*)"Models/Starbucks/Barras/VasoStarTres.obj");

	Model Tarro((char*)"Models/Starbucks/VitrinaDos/TarroSolo2.obj");
	Model TapaTarro((char*)"Models/Starbucks/VitrinaDos/TapaF.obj");
	/*Model GalletaUno((char*)"Models/Starbucks/TarroGalletas/GalletaUno.obj");
	Model GalletaDos((char*)"Models/Starbucks/TarroGalletas/GalletasDos.obj");
	Model GalletaTres((char*)"Models/Starbucks/TarroGalletas/GalletaTres.obj");
	Model GalletaCuatro((char*)"Models/Starbucks/TarroGalletas/GalletaCuatro.obj");
	Model GalletaCinco((char*)"Models/Starbucks/TarroGalletas/GalletaCinco.obj");
	Model GalletaSeis((char*)"Models/Starbucks/TarroGalletas/GalletaSeis.obj");*/

	Model MesaUno((char*)"Models/Starbucks/Mesas/MesaUno.obj");
	Model SillasT_DerUno((char*)"Models/Starbucks/Mesas/SillasT_DerUno.obj");
	Model SillasD_DerUno((char*)"Models/Starbucks/Mesas/SillasD_DerUno.obj");
	Model SillasT_IzqUno((char*)"Models/Starbucks/Mesas/SillasT_IzqUno.obj");
	Model SillasD_IzqUno((char*)"Models/Starbucks/Mesas/SillasD_IzqUno.obj");
	Model SillaSola_MesaUno((char*)"Models/Starbucks/Mesas/SillaSola_MesaUno.obj");
	Model CafeMesaUno((char*)"Models/Starbucks/Mesas/CafeMesaUno.obj");


	Model MesaDos((char*)"Models/Starbucks/Mesas/MesaDos.obj");
	Model SillasT_DerDos((char*)"Models/Starbucks/Mesas/SillasT_DerDos.obj");
	Model SillasD_DerDos((char*)"Models/Starbucks/Mesas/SillasD_DerDos.obj");
	Model SillasT_IzqDos((char*)"Models/Starbucks/Mesas/SillasT_IzqDos.obj");
	Model SillasD_IzqDos((char*)"Models/Starbucks/Mesas/SillasD_IzqDos.obj");
	Model CafeMesaDos((char*)"Models/Starbucks/Mesas/CafeMesaDos.obj");

	Model Malteada((char*)"Models/Starbucks/Postres/Malteada.obj");
	Model PastelBarra((char*)"Models/Starbucks/Postres/PastelBarra.obj");

	Model Pastel((char*)"Models/Starbucks/VitrinaDos/Pastel.obj");
	Model PastelDos((char*)"Models/Starbucks/VitrinaDos/Pastel2.obj");

	//PASTELES VITRINA
	Model PlatoPastel((char*)"Models/Starbucks/Postres/PlatoPastel.obj");
	Model Pastel_RebanadaUno((char*)"Models/Starbucks/Postres/Pastel_Rebanada1.obj");
	Model Pastel_RebanadaDos((char*)"Models/Starbucks/Postres/Pastel_Rebanada2.obj");
	/*Model Pastel_RebanadaTres((char*)"Models/Starbucks/Postres/Pastel_Rebanada3.obj");
	Model Pastel_RebanadaCuatro((char*)"Models/Starbucks/Postres/Pastel_Rebanada4.obj");
	Model Pastel_RebanadaCinco((char*)"Models/Starbucks/Postres/Pastel_Rebanada5.obj");
	Model Pastel_RebanadaSeis((char*)"Models/Starbucks/Postres/Pastel_Rebanada6.obj");*/

	Model CharolaUno((char*)"Models/Starbucks/Postres/CharolaUno.obj");
	Model Croissant1((char*)"Models/Starbucks/Postres/Croissant1.obj");
	Model Croissant2((char*)"Models/Starbucks/Postres/Croissant2.obj");
	Model Croissant3((char*)"Models/Starbucks/Postres/Croissant3.obj");
	Model Croissant4((char*)"Models/Starbucks/Postres/Croissant4.obj");
	Model Croissant5((char*)"Models/Starbucks/Postres/Croissant5.obj");
	Model Croissant6((char*)"Models/Starbucks/Postres/Croissant6.obj");
	Model Croissant7((char*)"Models/Starbucks/Postres/Croissant7.obj");
	Model Croissant8((char*)"Models/Starbucks/Postres/Croissant8.obj");
	Model Croissant9((char*)"Models/Starbucks/Postres/Croissant9.obj");

	Model CharolaDos((char*)"Models/Starbucks/Postres/CharolaDos.obj");
	Model cinnamon1((char*)"Models/Starbucks/Postres/cinnamon1.obj");
	Model cinnamon2((char*)"Models/Starbucks/Postres/cinnamon2.obj");
	Model cinnamon3((char*)"Models/Starbucks/Postres/cinnamon3.obj");
	/*Model cinnamon4((char*)"Models/Starbucks/Postres/cinnamon4.obj");
	Model cinnamon5((char*)"Models/Starbucks/Postres/cinnamon5.obj");
	Model cinnamon6((char*)"Models/Starbucks/Postres/cinnamon6.obj");
	Model cinnamon7((char*)"Models/Starbucks/Postres/cinnamon7.obj");*/

	Model CharolaTres((char*)"Models/Starbucks/Postres/CharolaTres.obj");
	Model pan1((char*)"Models/Starbucks/Postres/pan1.obj");
	Model pan2((char*)"Models/Starbucks/Postres/pan2.obj");
	/*Model pan3((char*)"Models/Starbucks/Postres/pan3.obj");
	Model pan4((char*)"Models/Starbucks/Postres/pan4.obj");
	Model pan5((char*)"Models/Starbucks/Postres/pan5.obj");*/


	//----------------------------------------------------------------PIZZERIA
	Model pizzeria((char*)"Models/pizzeria/pizzeria.obj");



	//---------------------------------------------------------------------NIKE
	Model nike((char*)"Models/fachadafinal/fachada.obj");
	Model maniquie((char*)"Models/maniquie/maniquie.obj");



	//--------------------------------------------------------------------BAÑOS

	Model Baños((char*)"Models/Baños/Baños.obj");


	//Inicialización de KeyFrames

	/*for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].incX = 0;
		KeyFrame[i].incY = 0;
		KeyFrame[i].incZ = 0;
	}*/

	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] ={
		// Positions            // Normals              // Texture Coords
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,  	1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f
	};

	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	GLuint indices[] =
	{  // Note that we start from 0!
		0,1,2,3,
		4,5,6,7,
		8,9,10,11,
		12,13,14,15,
		16,17,18,19,
		20,21,22,23,
		24,25,26,27,
		28,29,30,31,
		32,33,34,35
	};

	// Positions all containers
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Texture Coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	//SKYBOX------------------------------------------------------------------
	GLuint skyboxVBO, skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1,&skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices),&skyboxVertices,GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);

	//CARGAR SKYBOX-----------------------------------------------------------
	vector<const GLchar*> faces;
	faces.push_back("SkyBox/posx.jpg");
	faces.push_back("SkyBox/negx.jpg");
	faces.push_back("SkyBox/posy.jpg");
	faces.push_back("SkyBox/negy.jpg");
	faces.push_back("SkyBox/posz.jpg");
	faces.push_back("SkyBox/negz.jpg");
	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);
	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);
	
	// Game loop
	while (!glfwWindowShouldClose(window)){
		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		//animacion();
		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);
		
		//Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.9f, 0.9f, 0.9f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);

		//Pointlight 1
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), Light1.x, Light1.y, Light1.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), Light1.x, Light1.y, Light1.z);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.032f);

		//Pointlight 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), Light2.x, Light2.y, Light2.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), Light2.x, Light2.y, Light2.z);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.032f);

		//Pointlight 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), Light3.x, Light3.y, Light3.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), Light3.x, Light3.y, Light3.z);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.032f);

		//Pointlight 4
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), Light4.x, Light4.y, Light4.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), Light4.x, Light4.y, Light4.z);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.032f);

		//Spotlight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

		//Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);
		//Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();
		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		// Bind diffuse map
		//glBindTexture(GL_TEXTURE_2D, texture1);*/
		// Bind specular map
		/*glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);*/
		glBindVertexArray(VAO);
		glm::mat4 tmp = glm::mat4(1.0f); //Temp

	    //**********************                               ************************
	    //********************** D I B U J A R   M O D E L O S ************************
	    //**********************                               ************************
		
		//------------------------------------------------------------------FACHADA
		glm::mat4 model(1);
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTrasparencia"), 0);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Pisoasfalto.Draw(lightingShader);
		model = glm::mat4(1);
		Arbolsolo.Draw(lightingShader);
		Arbolesatras.Draw(lightingShader);
		Arbolesizquierda.Draw(lightingShader);
		Autosamarillo.Draw(lightingShader);
		Autosazul.Draw(lightingShader);
		Autosrojo.Draw(lightingShader);
		Bancasatras.Draw(lightingShader);
		Bardaperi.Draw(lightingShader);
		Casetasent.Draw(lightingShader);
		Escalaman.Draw(lightingShader);
		Escalawoman.Draw(lightingShader);
		Escalagirl.Draw(lightingShader);
		Escalaboy.Draw(lightingShader);
		Escalasatras.Draw(lightingShader);
		Escalasfrente.Draw(lightingShader);
		Escalasizquierda.Draw(lightingShader);
		Zeus.Draw(lightingShader);
		Espectacularplaza.Draw(lightingShader);
		Farolluz.Draw(lightingShader);
		Fuenteagua.Draw(lightingShader);
		Plantillapiso.Draw(lightingShader);
		Plumasparking.Draw(lightingShader);
		Rampaparking.Draw(lightingShader);
		entrada.Draw(lightingShader);

	    //----------------------------------------------------------------STAR WARS
		//glm::mat4 model(1);
		//view = camera.GetViewMatrix();
		//model = glm::mat4(1);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTrasparencia"), 0);
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Casamando.Draw(lightingShader);
		/*model = glm::mat4(1);
		Antenaoriginal.Draw(lightingShader);
		Antenasfuera.Draw(lightingShader);
		Antenaarriba.Draw(lightingShader);
		Arcoizquierdo.Draw(lightingShader);
		Arcoderecho.Draw(lightingShader);
		Barrildentro.Draw(lightingShader);
		Barrilfuera.Draw(lightingShader);
		Cascosfuera.Draw(lightingShader);
		Columnasizquierda.Draw(lightingShader);
		Columnasderecha.Draw(lightingShader);
		Domoarriba.Draw(lightingShader);
		Estantefuera.Draw(lightingShader);
		Lamparadentro.Draw(lightingShader);
		Lamparasfuera.Draw(lightingShader);
		Lonaafuera.Draw(lightingShader);
		Losaarriba.Draw(lightingShader);
		Mesadentro.Draw(lightingShader);
		Muroatras.Draw(lightingShader);
		Murofrenteizquierdo.Draw(lightingShader);
		Murofrentederecho.Draw(lightingShader);
		Muroizquierdo.Draw(lightingShader);
		Muroderecho.Draw(lightingShader);
		Naverazor.Draw(lightingShader);
		Panelpuertafrente.Draw(lightingShader);
		Panelverdeatras.Draw(lightingShader);
		Piedrasfuera.Draw(lightingShader);
		Pisofrente.Draw(lightingShader);
		Pisodentro.Draw(lightingShader);
		Pisofuera.Draw(lightingShader);
		Plantasadorno.Draw(lightingShader);
		Rielesfuera.Draw(lightingShader);
		Sillafuera.Draw(lightingShader);
		Sillasdentro.Draw(lightingShader);
		Sofadentro.Draw(lightingShader);
		Tazadentro.Draw(lightingShader);
		Tuboscurvosfuera.Draw(lightingShader);
		Ventanaizquierda.Draw(lightingShader);
		Ventanaderecha.Draw(lightingShader);
		Voladoarriba.Draw(lightingShader);
		Grogufuera.Draw(lightingShader);
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, movEsfera, movEsfera));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTrasparencia"), 0); //1 para activar la trasnparencia
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Esferagrogu.Draw(lightingShader);
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-0.5f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f,0.0f,desPuerta));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTrasparencia"), 0); //1 para activar la trasnparencia
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Puertafrente.Draw(lightingShader);
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Ranafuera.Draw(lightingShader);*/

		//----------------------------------------------------------------STARBUCKS

		//Carga de modelo fachada
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 1.5f,1.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		
		Fachada.Draw(lightingShader);

		PlantaUno.Draw(lightingShader);
		//PlantaDos.Draw(lightingShader);
		PlantaTres.Draw(lightingShader);

		Banca.Draw(lightingShader);
		Piso.Draw(lightingShader);
		CajaR.Draw(lightingShader);
		CafeteraDos.Draw(lightingShader);
		BarraPrin.Draw(lightingShader);
		BarraTrastes.Draw(lightingShader);
		BarraVasos.Draw(lightingShader);
		VasoStarUno.Draw(lightingShader);
		VasoStarDos.Draw(lightingShader);
		VasoStarTres.Draw(lightingShader);
		BarraFregadero.Draw(lightingShader);
		ManijaFregadero.Draw(lightingShader);


		MesaUno.Draw(lightingShader);

		SillasT_DerUno.Draw(lightingShader);
		SillasD_IzqUno.Draw(lightingShader);
		SillasD_DerUno.Draw(lightingShader);
		SillasT_IzqUno.Draw(lightingShader);
		SillaSola_MesaUno.Draw(lightingShader);
		CafeMesaUno.Draw(lightingShader);

		MesaDos.Draw(lightingShader);

		SillasT_DerDos.Draw(lightingShader);
		SillasD_IzqDos.Draw(lightingShader);
		SillasD_DerDos.Draw(lightingShader);
		SillasT_IzqDos.Draw(lightingShader);
		CafeMesaDos.Draw(lightingShader);

		Malteada.Draw(lightingShader);
		PastelBarra.Draw(lightingShader);

		Pastel.Draw(lightingShader);
		PastelDos.Draw(lightingShader);

		//PASTEL VITRINA
		PlatoPastel.Draw(lightingShader);
		Pastel_RebanadaUno.Draw(lightingShader);
		//Pastel_RebanadaDos.Draw(lightingShader);
		/*Pastel_RebanadaTres.Draw(lightingShader);
		Pastel_RebanadaCuatro.Draw(lightingShader);
		Pastel_RebanadaCinco.Draw(lightingShader);
		Pastel_RebanadaSeis.Draw(lightingShader);*/

		CharolaUno.Draw(lightingShader);
		CharolaDos.Draw(lightingShader);
		CharolaTres.Draw(lightingShader);


		Croissant1.Draw(lightingShader);
		Croissant2.Draw(lightingShader);
		Croissant3.Draw(lightingShader);
		/*Croissant4.Draw(lightingShader);
		Croissant5.Draw(lightingShader);
		Croissant6.Draw(lightingShader);
		Croissant7.Draw(lightingShader);
		Croissant8.Draw(lightingShader);
		Croissant9.Draw(lightingShader);*/

		cinnamon1.Draw(lightingShader);
		cinnamon2.Draw(lightingShader);
		cinnamon3.Draw(lightingShader);
		/*cinnamon4.Draw(lightingShader);
		cinnamon5.Draw(lightingShader);
		cinnamon6.Draw(lightingShader);
		cinnamon7.Draw(lightingShader);*/

		pan1.Draw(lightingShader);
		pan2.Draw(lightingShader);
		/*pan3.Draw(lightingShader);
		pan4.Draw(lightingShader);
		pan5.Draw(lightingShader);*/




		//FocosMenu.Draw(lightingShader);
		Menu.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 1.5f, 1.5f));
		model = glm::translate(model, glm::vec3(-4.806f, 1.770f, -0.111f));
		model = glm::rotate(model, glm::radians(angLlaveFreg), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotar en el eje Y
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		LlaveFregadero.Draw(lightingShader);
		glBindVertexArray(0);


		//-----------------------------------------------------------------------------------------------------------------------------------------
		//-----------------------------------------------------------------------------------------------------------------------------------------
		//								Carga de modelos CON TRANSPARENCIA
		//-----------------------------------------------------------------------------------------------------------------------------------------
		//-----------------------------------------------------------------------------------------------------------------------------------------


		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::mat4(1);
		//model = glm::translate(model, glm::vec3(posX, posY, posZ));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0); //Activacion de trasparencia
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 100.0);//Nivel de trasparecia con valor en color alpha

		Vitrina.Draw(lightingShader);
		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 1.0);
		glBindVertexArray(0);

		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 1.0);
		glBindVertexArray(0);

		//PUERTAS

		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::mat4(1);
		//model = glm::translate(model, glm::vec3(posX, posY, posZ));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);//Activacion de trasparencia
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 0.8);//Nivel de trasparecia con valor en color alpha
		puertas.Draw(lightingShader);
		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 1.0);
		glBindVertexArray(0);

		//PUERTA 1

		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::mat4(1);
		//model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(2.365f, 1.712f, 7.516f));
		model = glm::rotate(model, glm::radians(-abrir), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);//Activacion de trasparencia
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 0.4);//Nivel de trasparecia con valor en color alpha
		puertaUno.Draw(lightingShader);
		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 1.0);
		glBindVertexArray(0);

		//PUERTA DOS
		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::mat4(1);
		//model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(-2.455f, 1.671f, 7.468f));
		model = glm::rotate(model, glm::radians(abrir), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);//Activacion de trasparencia
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 0.4);//Nivel de trasparecia con valor en color alpha
		puertaDos.Draw(lightingShader);
		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 1.0);
		glBindVertexArray(0);

		//PUERTA3

		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::mat4(1);
		//model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(-2.48f, 1.658f, -7.475f));
		model = glm::rotate(model, glm::radians(abrirDos), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);//Activacion de trasparencia
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 0.4);//Nivel de trasparecia con valor en color alpha
		puertaTres.Draw(lightingShader);
		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 1.0);
		glBindVertexArray(0);


		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::mat4(1);
		//model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(2.314f, 1.671f, -7.452f));
		model = glm::rotate(model, glm::radians(-abrirDos), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);//Activacion de trasparencia
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 0.4);//Nivel de trasparecia con valor en color alpha
		puertaCuatro.Draw(lightingShader);
		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 1.0);
		glBindVertexArray(0);

		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::mat4(1);
		//model = glm::translate(model, glm::vec3(posX, posY, posZ));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);//Activacion de trasparencia
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 0.8);//Nivel de trasparecia con valor en color alpha
		Vidrio.Draw(lightingShader);
		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 1.0);
		glBindVertexArray(0);



		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::mat4(1);
		//model = glm::translate(model, glm::vec3(posX, posY, posZ));
		//model = glm::translate(model, glm::vec3(-4.435f, 2.128f, 3.952f));
		model = glm::rotate(model, glm::radians(vibrar), glm::vec3(0.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);//Activacion de trasparencia
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 100.0);//Nivel de trasparecia con valor en color alpha
		cafetera.Draw(lightingShader);
		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 1.0);
		glBindVertexArray(0);

		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::mat4(1);
		//model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(-4.228f, 1.925f, 3.96f));
		model = glm::translate(model, glm::vec3(-x, -y, z));
		model = glm::rotate(model, glm::radians(-TazaR), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(TazaRY), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);//Activacion de trasparencia
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 100.0);//Nivel de trasparecia con valor en color alpha
		Taza.Draw(lightingShader);
		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 1.0);
		glBindVertexArray(0);

		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::mat4(1);
		//model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(-1.837f, 2.277f, -2.195f));
		model = glm::rotate(model, glm::radians(TarroR), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);//Activacion de trasparencia
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 0.7);//Nivel de trasparecia con valor en color alpha
		Tarro.Draw(lightingShader);
		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 1.0);
		glBindVertexArray(0);



		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::mat4(1);
		//model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(-1.836f, 2.431f, -2.179f));
		model = glm::translate(model, glm::vec3(xT, -yT, -zT));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);//Activacion de trasparencia
		//glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 100.0);//Nivel de trasparecia con valor en color alpha  TAPA NEGRA
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 0.7);//Nivel de trasparecia con valor en color alpha TAPA TRANSPARENTE
		TapaTarro.Draw(lightingShader);
		//Tapa.Draw(lightingShader);
		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 1.0);
		glBindVertexArray(0);

		//glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//model = glm::mat4(1);
		////model = glm::translate(model, glm::vec3(posX, posY, posZ));
		//model = glm::translate(model, glm::vec3(-1.832, 2.172f, -2.195));
		//model = glm::translate(model, glm::vec3(xGU, -yGU, -zGU));
		//model = glm::rotate(model, glm::radians(rGU), glm::vec3(1.0f, 0.0f, 0.0f));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);//Activacion de trasparencia
		//glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 100.0);//Nivel de trasparecia con valor en color alpha
		//GalletaUno.Draw(lightingShader);
		//glDisable(GL_BLEND);  //Desactiva el canal alfa 
		//glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 1.0);
		//glBindVertexArray(0);

		//glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//model = glm::mat4(1);
		//model = glm::translate(model, glm::vec3(posX, posY, posZ));
		//model = glm::translate(model, glm::vec3(-1.832, 2.191f, -2.195));
		//model = glm::translate(model, glm::vec3(xGD, -yGD, -zGD));
		//model = glm::rotate(model, glm::radians(rGD), glm::vec3(1.0f, 0.0f, 0.0f));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);//Activacion de trasparencia
		//glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 100.0);//Nivel de trasparecia con valor en color alpha
		//GalletaDos.Draw(lightingShader);
		//glDisable(GL_BLEND);  //Desactiva el canal alfa 
		//glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 1.0);
		//glBindVertexArray(0);

		//glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//model = glm::mat4(1);
		//model = glm::translate(model, glm::vec3(posX, posY, posZ));
		//model = glm::translate(model, glm::vec3(-1.832, 2.21f, -2.195));
		//model = glm::translate(model, glm::vec3(xGT, -yGT, -zGT));
		//model = glm::rotate(model, glm::radians(rGT), glm::vec3(1.0f, 0.0f, 0.0f));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);//Activacion de trasparencia
		//glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 100.0);//Nivel de trasparecia con valor en color alpha
		//GalletaTres.Draw(lightingShader);
		//glDisable(GL_BLEND);  //Desactiva el canal alfa 
		//glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 1.0);
		//glBindVertexArray(0);

		//glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//model = glm::mat4(1);
		//model = glm::translate(model, glm::vec3(posX, posY, posZ));
		//model = glm::translate(model, glm::vec3(-1.832, 2.229f, -2.195));
		//model = glm::translate(model, glm::vec3(xGC, -yGC, -zGC));
		//model = glm::rotate(model, glm::radians(rGC), glm::vec3(1.0f, 0.0f, 0.0f));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);//Activacion de trasparencia
		//glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 100.0);//Nivel de trasparecia con valor en color alpha
		//GalletaCuatro.Draw(lightingShader);
		//glDisable(GL_BLEND);  //Desactiva el canal alfa 
		//glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 1.0);
		//glBindVertexArray(0);

		//glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//model = glm::mat4(1);
		//model = glm::translate(model, glm::vec3(posX, posY, posZ));
		//model = glm::translate(model, glm::vec3(-1.832, 2.248f, -2.195));
		//model = glm::translate(model, glm::vec3(xGCI, -yGCI, -zGCI));
		//model = glm::rotate(model, glm::radians(rGCI), glm::vec3(1.0f, 0.0f, 0.0f));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);//Activacion de trasparencia
		//glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 100.0);//Nivel de trasparecia con valor en color alpha
		//GalletaCinco.Draw(lightingShader);
		//glDisable(GL_BLEND);  //Desactiva el canal alfa 
		//glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 1.0);
		//glBindVertexArray(0);

		//glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//model = glm::mat4(1);
		//model = glm::translate(model, glm::vec3(posX, posY, posZ));
		//model = glm::translate(model, glm::vec3(-1.832, 2.267f, -2.195));
		//model = glm::translate(model, glm::vec3(xGS, -yGS, -zGS));
		//model = glm::rotate(model, glm::radians(rGS), glm::vec3(1.0f, 0.0f, 0.0f));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);//Activacion de trasparencia
		//glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 100.0);//Nivel de trasparecia con valor en color alpha
		//GalletaSeis.Draw(lightingShader);
		//glDisable(GL_BLEND);  //Desactiva el canal alfa 
		//glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 1.0);
		//glBindVertexArray(0);






		//----------------------------------------------------------------PIZZERIA
		model = glm::mat4(1);
		model = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTrasparencia"), 0);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		pizzeria.Draw(lightingShader);



		//---------------------------------------------------------------------NIKE
		model = glm::mat4(1);
		model = glm::translate(glm::mat4(1.0), glm::vec3(33.3f, 4.347, -12.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTrasparencia"), 0);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		nike.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(glm::mat4(1.0), glm::vec3(32.9f, 5.1f, -12.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		model = glm::rotate(model, 23 * glm::radians((float)glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		maniquie.Draw(lightingShader);





		//--------------------------------------------------------------------BAÑOS
		
		model = glm::mat4(1);
		/*model = glm::translate(glm::mat4(1.0), glm::vec3(-4.560f, 0.460f, -3.042f));
		model = glm::scale(model, glm::vec3(0.804f, 0.804f, 0.804f));*/
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTrasparencia"), 0);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Baños.Draw(lightingShader);



		//Traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::mat4(1);
		model = glm::scale(model, glm::vec3(1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		//objTras.Draw(lightingShader);
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glBindVertexArray(0);
		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");
		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		//model = glm::translate(model, lightPos);
		//model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		glBindVertexArray(lightVAO);
		for (GLuint i = 0; i < 4; i++){
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);
		// Draw skybox as last
		glDepthFunc(GL_LEQUAL);
		SkyBoxshader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		//SKYBOX CUBO-------------------------------------------------
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);
	glfwTerminate();
	return 0;
}

void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode){
	if (keys[GLFW_KEY_H]) {
		if (desPuerta == 4.0f || desPuerta == 0.0f) {
			abrirDoor = !abrirDoor;
		}
	}
	if (keys[GLFW_KEY_G]) {
		if (movEsfera == 4.0f || movEsfera == 0.0f) {
			usarFuerza = !usarFuerza;
		}
	}
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action){
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key >= 0 && key < 1024){
		if (action == GLFW_PRESS){
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE){
			keys[key] = false;
		}
	}
	//ACTIVAR LAS LUCES-------------------------------------
	if (keys[GLFW_KEY_V]){
		active = !active;
		if (active)
			Light1 = glm::vec3(1.0f, 1.0f, 1.0f);
		else
			Light1 = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	if (keys[GLFW_KEY_B]){
		active = !active;
		if (active)
			Light2 = glm::vec3(0.0f, 0.0f, 1.0f);
		else
			Light2 = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	if (keys[GLFW_KEY_N]){
		active = !active;
		if (active)
			Light3 = glm::vec3(0.0f, 0.0f, 1.0f);
		else
			Light3 = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	if (keys[GLFW_KEY_M]){
		active = !active;
		if (active)
			Light4 = glm::vec3(1.0f, 0.0f, 0.0f);
		else
			Light4 = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	if (keys[GLFW_KEY_3]) {
		animTres = !animTres;
		animT = !animT;
	}
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos){
	if (firstMouse){
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}
	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;
	lastX = xPos;
	lastY = yPos;
	camera.ProcessMouseMovement(xOffset, yOffset);
}

void DoMovement(){
	if (abrirDoor){
		if (desPuerta < 4.0f){
			desPuerta += 0.5f;
		}
	}
	else{
		if (desPuerta > 0.0f){
			desPuerta -= 0.5f;
		}
	}
	if (usarFuerza){
		if (movEsfera < 4.0f){
			movEsfera += 0.5f;
		}
	}
	else{
		if (movEsfera > 0.0f){
			movEsfera -= 0.5f;
		}
	}
	if (keys[GLFW_KEY_1]){
		movCamera = 0.01f;
	}
	//CONTROLES DE LA CAMARA-------------------------------
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]){
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]){
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]){
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]){
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}

	//Animacion Starbucks Taza cae de cafetera y roda

	if (animTres) {

		if (vibrar > -0.8f && sentidoTres == true) {
			vibrar -= 0.2f;
			if (vibrar <= -0.8f)
				sentidoTres = false;//Desativacion para cambiar el sentido del rotacion 
		}



		if (vibrar < 0.8f && sentidoTres == false) {
			vibrar += 0.2f;
			if (vibrar >= 0.8f)
				sentidoTres = true;//Ativacion para cambiar el sentido de rotacion 
		}

	}

	if (animT) {
		bool avanza = false;

		if (x <= 0 && x > -0.300) {
			x -= 0.0005;
		}
		else {
			avanza = true;
		}

		if (avanza) {
			if (y <= 1.73) {     //***************************************
				TazaR = 90;
				y += 0.020;
			}
			else if (z <= 4 && TazaRY < 90) {
				TazaRY += 0.50;
				z += 0.015;
			}
		}

	}

}