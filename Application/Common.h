/*
This file is part of EasyFlow.

EasyFlow is free software : you can redistribute it and / or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

EasyFlow is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with EasyFlow. If not, see <http://www.gnu.org/licenses/>.

Author: Toby Chen @ 2014
*/

#pragma once
#define APP_NAME "EasyFlow"
#define APP_VERSION "TechPreview"
#define MAX_STRING_FRAGMENTS 4096

class CMainFrame;
class CFlowGraph;
class CFlowEditor;
class CToolBox;
class CNodePropertyUI;
class CGeneratorContext;
struct CGlobalEnvironment
{
	CMainFrame* MainFrame;
	CToolBox* ToolBox;
	CFlowGraph* FlowGraph;
	CFlowEditor* FlowEditor;
	CNodePropertyUI* PropCtrl;
	CGeneratorContext* GeneratorContext;
};

extern CGlobalEnvironment* gEnv;

#define ERROR_STRING "!ERROR!"
