#pragma once

static int loop = 0;

class SingletonRenderWindow
{

public:
	//SingletonRenderWindow(const SingletonRenderWindow&) = delete;
	//SingletonRenderWindow(SingletonRenderWindow&&) = delete;
	//SingletonRenderWindow& operator=(const SingletonRenderWindow&) = delete;
	//SingletonRenderWindow& operator=(SingletonRenderWindow&&) = delete;

	static SingletonRenderWindow& Get()
	{
		static SingletonRenderWindow RenderWindowInstance;
		return RenderWindowInstance;
	}

	static SingletonRenderWindow& Set(GLFWwindow* Input)
	{
		static SingletonRenderWindow& RenderWindowInstance = Get();
		if (loop == 0)
		{
			RenderWindowSet(RenderWindowInstance, Input);
		}	
			return RenderWindowInstance;
	}

	static GLFWwindow* RenderWindowGet() 
	{ 
		return Get().RenderWindowInternalGet(); 
	}

	static SingletonRenderWindow& RenderWindowSet(SingletonRenderWindow& Input, GLFWwindow* windowin)
	{ 
		loop++;
		Input.get_RenderWindow = Set(windowin).RenderWindowInternalSet(windowin, Input);
		return Input;
	}
	
private:
	SingletonRenderWindow() {}

	GLFWwindow* get_RenderWindow;

	GLFWwindow* RenderWindowInternalGet()
	{ 
		return get_RenderWindow; 
	}

	GLFWwindow* RenderWindowInternalSet(GLFWwindow* saveRenderWindow, SingletonRenderWindow& Input)
	{
		Input.get_RenderWindow = saveRenderWindow;
		return Input.get_RenderWindow;
	}
};


