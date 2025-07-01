#include <iostream>
#include <glad/glad.h> // GLAD carga los punteros de funciones de OpenGL seg�n la versi�n que usamos
#include <GLFW/glfw3.h> // GLFW gestiona ventanas, inputs, y contexto de OpenGL

// ===================== SHADERS (programas que corren en la GPU) =====================

// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"  // Versi�n de GLSL que se usar� (3.30 core profile)
"layout (location = 0) in vec3 aPos;\n"                 // Entrada del shader: un vec3 (posici�n del v�rtice) en la ubicaci�n 0
"void main()\n"                                         // Funci�n principal obligatoria en un shader
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n" // Se establece la posici�n del v�rtice en coordenadas homog�neas (vec4)
"}\0";

// Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"// Versi�n de GLSL
"out vec4 FragColor;\n"                                 // Salida del shader: el color del fragmento
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"       // Se asigna un color RGBA al fragmento (naranja oscuro)
"}\n\0";


int main() {
	
	// ===================== INICIALIZACI�N DE GLFW =====================
	// Initialize GLFW (necesario antes de usar cualquier funci�n de GLFW)
	glfwInit();

	// Tell GLFW what version of OpenGL we are using, in this case is OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using CORE profile, that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	// ===================== DEFINICI�N DE V�RTICES =====================
	GLfloat vertices[] =
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner // V�rtice 1: (x, y, z)
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f // Upper corner
	};

	// ===================== CREACI�N DE VENTANA =====================
	// Create a GLFWwindow object of 800x800 pixels
	// Par�metros: ancho, alto, t�tulo, monitor (null para modo ventana), contexto compartido (null)
	GLFWwindow* window = glfwCreateWindow(800, 800, "OpenGl", NULL, NULL);
	// Error check
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Hace que la ventana actual sea el contexto activo de OpenGL
	glfwMakeContextCurrent(window);

	// Load GLAD so it configures OpenGL (carga funciones de OpenGL seg�n la GPU)
	gladLoadGL();

	// Specify the viewport of OpenGL in the window
	// Esto dice que OpenGL puede dibujar desde(0, 0) hasta(800, 800)
	glViewport(0, 0, 800, 800);

	// ===================== CREACI�N Y COMPILACI�N DE SHADERS =====================
	// Vertex Shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);                      // Crea objeto shader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);                 // Asigna c�digo fuente
	glCompileShader(vertexShader);                                              // Compila

	// Fragment Shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);                 // Igual que arriba
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Shader Program (donde se combinan todos los shaders)
	GLuint shaderProgram = glCreateProgram();                                   // Crea programa
	glAttachShader(shaderProgram, vertexShader);                                // Adjunta shaders
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);                                               // Enlaza

	// Libera memoria de shaders individuales (ya est�n en el programa)
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// ===================== BUFFERS PARA MANDAR DATOS A LA GPU =====================

	GLuint VAO, VBO;                                 // Vertex Array Object y Vertex Buffer Object

	glGenVertexArrays(1, &VAO);                      // Crea 1 VAO
	glGenBuffers(1, &VBO);                           // Crea 1 VBO

	glBindVertexArray(VAO);                          // Enlaza el VAO (se activan sus configuraciones)

	glBindBuffer(GL_ARRAY_BUFFER, VBO);              // Enlaza el VBO al tipo GL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Carga los datos de v�rtices en el buffer
	// Par�metros: tipo, tamanio en bytes, puntero a los datos, uso previsto:
	// GL_STATIC_DRAW -> no se modifican (ideal para objetos est�ticos)
	// GL_DYNAMIC_DRAW -> se modifican a menudo
	// GL_STREAM_DRAW -> se usan una vez

	// Configura c�mo interpretar los datos de v�rtice
	glVertexAttribPointer(
		0,                  // �ndice del atributo (coincide con layout(location=0) en el shader)
		3,                  // Cantidad de valores por v�rtice (x, y, z)
		GL_FLOAT,           // Tipo de dato
		GL_FALSE,           // Normalizar (GL_TRUE lo escala entre 0 y 1 si es int)
		3 * sizeof(float),  // Tamanio de un v�rtice completo
		(void*)0            // Offset inicial en el buffer
	);
	glEnableVertexAttribArray(0);  // Habilita el atributo de v�rtice en la posici�n 0

	// Desenlazamos el buffer para evitar cambios accidentales
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	// specify the color of the background
	// especificamos el color de fondo (RGBA)
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	// Borramos el color del "back buffer" y lo llenamos con el color definido arriba
    // El "back buffer" es una imagen que a�n no se muestra en pantalla, lo que evita parpadeos
	glClear(GL_COLOR_BUFFER_BIT);
	// Swap the back buffer with the front buffer
	glfwSwapBuffers(window);

	// ===================== LOOP PRINCIPAL DE RENDERIZADO =====================
	// Main while loop
	while (!glfwWindowShouldClose(window)) // Mientras no se cierre la ventana
	{
		// Limpia el "back buffer" con el color de fondo
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);     // Color de fondo (oscuro)
		glClear(GL_COLOR_BUFFER_BIT);               // Limpia el buffer de color

		glUseProgram(shaderProgram);                // Activa el programa de shaders
		glBindVertexArray(VAO);                     // Enlaza el VAO para usar su configuraci�n
		glDrawArrays(GL_TRIANGLES, 0, 3);           // Dibuja: modo, inicio, cantidad de v�rtices

		glfwSwapBuffers(window); // Intercambia el front buffer y back buffer

		// glfwPollEvents se encarga de detectar e interpretar eventos como teclado, mouse, etc.
		glfwPollEvents(); // Take care of all GLFW events
		// Nota: En este programa no redibujamos nada en cada frame, pero en uno m�s complejo
		// aqu� se actualizar�a la escena y se llamar�a a glClear, glfwSwapBuffers, etc.
	}

	// ===================== LIBERAR RECURSOS =====================
	glDeleteVertexArrays(1, &VAO);                  // Borra el VAO
	glDeleteBuffers(1, &VBO);                       // Borra el VBO
	glDeleteProgram(shaderProgram);                 // Borra el shader program

	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program Muy importante para liberar memoria
	glfwTerminate();
	return 0;
}