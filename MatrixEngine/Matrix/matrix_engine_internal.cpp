#include "matrix_engine_internal.h"
#include <stdio.h>

#if _MSC_VER >= 1400
extern "C" { FILE __iob_func[3] = { *stdin,*stdout,*stderr }; }
#endif

using namespace MatrixEngine;
using namespace MatrixEngine::Core;

std::ofstream IO::Log::error_stream;
std::ofstream IO::Log::log_stream;

Core::Device *Core::_pCurrentDevice = NULL;
Scene::SceneGraph *Scene::_pCurrentSceneGraph = NULL;

void MatrixEngine::Core::IO::Log::WriteOnStream(std::string msg, std::ofstream & os) {
	SDL_Log(msg.c_str());
	os << msg;
}

char * MatrixEngine::Core::IO::Log::TextFileRead(std::string filepath)
{
	char *text = NULL;

	if (filepath.c_str() != NULL)
	{
		FILE *file;
#ifndef _WIN32
        	file = fopen(filepath.c_str(), "rt");
#else
		fopen_s(&file, filepath.c_str(), "rt");
#endif
		if (file != NULL) {
			fseek(file, 0, SEEK_END);
			int count = ftell(file);
			rewind(file);

			if (count > 0) {
				text = (char*)malloc(sizeof(char)*(count + 1));
				count = fread(text, sizeof(char), count, file);
				text[count] = '\0';
			}
			fclose(file);
		}
	}
	return text;
}
