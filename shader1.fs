#version 330 core
	out vec4 FragColor;
	in vec4 vertexColor; //comments are ok?
	void main()
	{
	   FragColor = vertexColor;
	}