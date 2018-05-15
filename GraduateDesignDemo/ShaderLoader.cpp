#include "ShaderLoader.h"
#include "Logger.h"

// --------------------------------------------------------------------------------------------
Shader ShaderLoader::Load(std::string name, std::string vsPath, std::string fsPath, std::vector<std::string> defines)
{
	std::ifstream vsFile, fsFile;
	vsFile.open(vsPath);
	fsFile.open(fsPath);

	if (!vsFile.is_open() || !fsFile.is_open())
	{
		Log::Message("Shader failed to load at path: " + vsPath + " and " + fsPath, LOG_ERROR);
		return Shader();
	}

	std::string vsDirectory = vsPath.substr(0, vsPath.find_last_of("/\\"));
	std::string fsDirectory = fsPath.substr(0, fsPath.find_last_of("/\\"));
	std::string vsSource = readShader(vsFile, name, vsPath);
	std::string fsSource = readShader(fsFile, name, fsPath);

	Shader shader(name, vsSource, fsSource, defines);

	vsFile.close();
	fsFile.close();

	return shader;
}
// --------------------------------------------------------------------------------------------
std::string ShaderLoader::readShader(std::ifstream& file, const std::string& name, std::string path)
{
	int index = path.find_last_of("/");
	std::string directory;
	if (index >= 0)
	{
		directory = path.substr(0, path.find_last_of("/\\"));
	}
	else
	{
		directory = "";
	}
	//	 directory = path.substr(0, path.find_last_of("/\\"));
	std::string source, line;
	while (std::getline(file, line))
	{
		if (line.substr(0, 8) == "#include")
		{
			std::string includePath = directory + "/" + line.substr(9);

			if (directory == "")
			{
				includePath = line.substr(9);
			}
			else
			{
				includePath = directory + "/" + line.substr(9);
			}
			std::ifstream includeFile(includePath);
			if (includeFile.is_open())
			{
				source += readShader(includeFile, name, includePath);
			}
			else
			{
				Log::Message("Shader: " + name + ": include: " + includePath + " failed to open.", LOG_ERROR);
			}
			includeFile.close();
		}
		else
			source += line + "\n";
	}
	return source;
}