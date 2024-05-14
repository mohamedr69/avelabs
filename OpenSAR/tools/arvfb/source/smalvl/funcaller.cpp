/* Copyright(C) 2013, OpenSAR by Fan Wang(parai). All rights reserved.
 *
 * This file is part of OpenSAR.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * Email: parai@foxmail.com
 * Source Open At: https://github.com/parai/OpenSAR/
 */
#include "funcaller.h"
#include "runtime.h"
#include <fstream>
#include "arvfb.h"

funcaller_t::funcaller_t()
{
}

funcaller_t::~funcaller_t()
{
	extensions_list_t::iterator i;
	printf("DELETE OF FUNCALL\n");
	for (i = extensions.begin(); i != extensions.end(); i++)
	{
		//TODO:
	}
}

bool funcaller_t::open_extension(require_t* require) throw (nativelib_exception_t)
{


	lib_extension_t ext;
	ext.lib_name = require->get_name();

	void* handle = dlopen(ext.lib_name.data(), RTLD_LAZY);

	if (handle == NULL)
	{
		std::cerr << dlerror() << std::endl;
		return false;
	}

	ext.handler = handle;

	//require->print();

	std::list<expr_t*>& declares = require->get_declares();

	for(std::list<expr_t*>::iterator i=declares.begin(); i!=declares.end(); i++)
	{
		meta_info_t meta;
		declare_t* declare = dynamic_cast<declare_t*>(*i);
		//declare->print();
		if(declare)
		{
			type_t* type = dynamic_cast<type_t*>(declare->get_type());
			if(type)
			{
				meta.return_type = type->get_value();
			}
			else
			{
				printf("Require Error1.\n");
			}
			meta.name = declare->get_name();
			for(std::list<expr_t*>::iterator j=declare->get_args().begin(); j!=declare->get_args().end(); j++)
			{
				type_t* type = dynamic_cast<type_t*>(*j);
				if(type)
				{
					meta.argument_types.push_back(type->get_value());
				}
				else
				{
					printf("Require Error2.\n");
				}
			}

			ext.functions_list.push_back(meta);
		}
		else
		{
			printf("Require Error3.\n");
		}
	}

	extensions.push_back(ext);
	RUNTIME_DEBUG("Loading native extension %s Done.", ext.lib_name.data());
	return true;
}

ref_t funcaller_t::call(void* faddr, const meta_info_t& meta,
		std::vector<object_t*>& objs) throw (runtime_exception_t)
{
	if (objs.size() != meta.argument_types.size()
			&& meta.argument_types.back() != VA)
		throw runtime_exception_t(
				std::string("Function ") + meta.name
						+ " invalid argumets count",
				FUNCTION__CALL_INVALID_ARGUMENTS);

	/* Arguments count = 0 */
	if (objs.size() == 0)
	{
		if (meta.return_type == NONE)
		{
			void (*fun)() = (void (*)()) faddr;
			fun();
			return REF_IS_VOID;
		}
		if (meta.return_type == INTEGER)
		{
			int (*fun)() = (int (*)()) faddr;
			ref_t ref = heap_manager_t::get_instance()->add_object(INTEGER);
			object_t* obj = heap_manager_t::get_instance()->get_object(ref);
			obj->i = fun();
			return ref;
		}
		else if (meta.return_type == STRING)
		{
			const char* (*fun)() = (const char* (*)()) faddr;
			ref_t ref = heap_manager_t::get_instance()->add_object(STRING);
			object_t* obj = heap_manager_t::get_instance()->get_object(ref);
			obj->s = new std::string(fun());
			return ref;
		}
		else if (meta.return_type == FLOATPOINT)
		{
			double (*fun)() = (double (*)()) faddr;
			ref_t ref = heap_manager_t::get_instance()->add_object(FLOATPOINT);
			object_t* obj = heap_manager_t::get_instance()->get_object(ref);
			obj->d = fun();
			return ref;
		}
	}
	/* Arguments count = 1 */
	else if (objs.size() == 1)
	{
		//fun(const char*)
		if (meta.argument_types[0] == STRING)
		{
			if (meta.return_type == INTEGER)
			{
				int (*fun)(const char*) = (int (*)(const char*)) faddr;
				ref_t ref = heap_manager_t::get_instance()->add_object(INTEGER);
				object_t* obj = heap_manager_t::get_instance()->get_object(ref);
				obj->i = fun(((const char*) *objs[0]));
				return ref;
			}
			else if (meta.return_type == NONE)
			{
				void (*fun)(const char*) = (void (*)(const char*)) faddr;
				fun((const char*) *objs[0]);
				return 0x0;
			}
			else if (meta.return_type == STRING)
			{
				char buffer[BUFFER_SIZE];
				memset(buffer, 0x0, sizeof(buffer));
				const char* src = (const char*) *objs[0];
				memcpy(buffer, src, strlen(src));
				char* (*fun)(char*) = (char* (*)(char*)) faddr;
				ref_t ref = heap_manager_t::get_instance()->add_object(STRING);
				object_t* obj = heap_manager_t::get_instance()->get_object(ref);
				obj->s = new std::string(fun(buffer));
				return ref;
			}
		}
		//fun(int)
		else if (meta.argument_types[0] == INTEGER)
		{
			if (meta.return_type == INTEGER)
			{
				int (*fun)(int) = (int (*)(int)) faddr;
				ref_t ref = heap_manager_t::get_instance()->add_object(INTEGER);
				object_t* obj = heap_manager_t::get_instance()->get_object(ref);
				obj->i = fun((int) *objs[0]);
				return ref;
			}
		}
	}
	/*Arguments count == 2 */
	else if (objs.size() == 2)
	{
		if (meta.return_type == INTEGER)
		{
			if (meta.argument_types[1] == STRING
					&& meta.argument_types[0] == STRING)
			{
				int (*fun)(const char*,
						const char*) = ( int (*)(const char*, const char*) ) faddr;
				ref_t ref = heap_manager_t::get_instance()->add_object(INTEGER);
				object_t* obj = heap_manager_t::get_instance()->get_object(ref);
				RUNTIME_DEBUG("Call function %s(\"%s\", \"%s\")",
						meta.name.data(), (const char*) *objs[1],
						(const char*) *objs[0]);
				obj->i = fun((const char*) *objs[1], (const char*) *objs[0]);
				return ref;
			}
			else if (meta.argument_types[1] == STRING
					&& meta.argument_types[0] == INTEGER)
			{
				int (*fun)(const char*,
						int) = ( int (*)(const char*, int) ) faddr;
				ref_t ref = heap_manager_t::get_instance()->add_object(INTEGER);
				object_t* obj = heap_manager_t::get_instance()->get_object(ref);
				RUNTIME_DEBUG("Call function %s(\"%s\", \"%d\")",
						meta.name.data(), (const char*) *objs[1],
						(int) *objs[0]);
				obj->i = fun((const char*) *objs[1], (int) *objs[0]);
				return ref;
			}
		}
	}
	/* Arguments count == 3 */
	else if (objs.size() == 3)
	{
		if (meta.return_type == STRING)
		{
			if (meta.argument_types[2] == STRING
					&& meta.argument_types[1] == INTEGER
					&& meta.argument_types[0] == INTEGER)
			{
				const char* (*fun)(const char*, int,
						int) = ( const char* (*)(const char*, int, int) ) faddr;
				int size = (int) *objs[1];
				char buffer[size];
				memset(buffer, 0x0, size);
				fun(buffer, size, (int) *objs[0]);
				//TODO: memcontrol
				ref_t ref = heap_manager_t::get_instance()->add_object(STRING);
				object_t* obj = heap_manager_t::get_instance()->get_object(ref);
				obj->s = new std::string(buffer);
				return ref;
			}
		}
	}
	/* Arguments count = 4 */
	else if (objs.size() == 4)
	{
		if (meta.return_type == INTEGER)
		{
			if (meta.argument_types[3] == STRING
					&& meta.argument_types[2] == INTEGER
					&& meta.argument_types[1] == INTEGER
					&& meta.argument_types[0] == INTEGER)
			{
				int (*fun)(const char*, int, int,
						int) = (int (*)(const char*, int, int, int)) faddr;
				ref_t ref = heap_manager_t::get_instance()->add_object(
						meta.return_type);
				object_t* obj = heap_manager_t::get_instance()->get_object(ref);
				obj->i = fun((const char*) *objs[0], (int) *objs[1],
						(int) *objs[2], (int) *objs[3]);
				return ref;
			}
		}
	}

	//TODO:
	runtime_exception_t e(FUNCTION__CALL_INVALID_ARGUMENTS);
	e.get_message() += std::string(" ") + std::string(__FILE__)
			+ std::string("::") + std::to_string(__LINE__) + "::  ";
	e.get_message() += dt::type_to_string(meta.return_type);
	e.get_message() += " ";
	e.get_message() += meta.name;
	e.get_message() += " (";
	for (int i = 0; i < (int) objs.size(); i++)
	{
		e.get_message() += dt::type_to_string(meta.argument_types[i]);
		e.get_message() += " ";
	}
	e.get_message() += ") ";
	throw e;
}

ref_t funcaller_t::call_function(function_call_t* fc,
		std::vector<object_t*>& objs) throw (runtime_exception_t)
{
	/* Clear any existing error*/

	std::list<meta_info_t>::iterator fun_meta;

	for (extensions_list_t::iterator i = extensions.begin();
			i != extensions.end(); i++)
	{
		for (fun_meta = (*i).functions_list.begin();
				fun_meta != (*i).functions_list.end(); fun_meta++)
		{
			//std::cout << "*\t" << "looking " << (*fun_meta).name << std::endl;

			if ((*fun_meta).name == fc->get_name())
			{
				dlerror();
				void* handle = (*i).handler;
				void* fun_addr = dlsym(handle, (*fun_meta).name.c_str());

				if (dlerror() == NULL)
				{
					RUNTIME_DEBUG("Call function %s from native lib",
							(*fun_meta).name.c_str());
					return call(fun_addr, (*fun_meta), objs);
				}
			}
		}
	}
	/*Preparing exception message */
	runtime_exception_t e(FUNCTION_NOT_DECL);
	e.get_message() += std::string(" '") + fc->get_name() + std::string("' ");
	throw e;
}

void funcaller_t::print_meta(meta_info_t& meta)
{
	std::cout << "*\tPrinting meta information for function:" << meta.name
			<< std::endl;
	std::vector<TYPE>::iterator i = meta.argument_types.begin();

	std::cout << "*\tfunction return type "
			<< dt::type_to_string(meta.return_type) << std::endl;
	std::cout << "*\targuments type" << std::endl;

	for (; i != meta.argument_types.end(); i++)
	{
		std::cout << "*\t\t" << dt::type_to_string((*i)) << std::endl;
	}

	std::cout << std::endl;
}
static void print_obj(object_t* obj)
{
	switch(obj->get_type())
	{
		case INTEGER:
			Arch_Trace("%d",obj->i);
			break;
		case FLOATPOINT:
			Arch_Trace("%lf",obj->d);
			break;
		case ARRAY:
		{
			array_obj_t* array_map = obj->a;
			Arch_Trace("dict[");
			for(std::map<std::string, ref_t>::iterator i=array_map->begin();i!=array_map->end();i++)
			{
				Arch_Trace("%s=",(*i).first.c_str());
				object_t* obj = runtime_t::get_instance()->get_mem()->get_object((*i).second);
				print_obj(obj);
				Arch_Trace(", ");
			}
			Arch_Trace("]");
			break;
		}
		case BOOL:
			Arch_Trace("%s",obj->b?"True":"False");
			break;
		case STRING:
			Arch_Trace("%s",obj->s->c_str());
			break;
		case NONE:
			Arch_Trace("None");
			break;
		default:
			break;
	}

}
void funcaller_t::Print(std::vector<object_t*>& objs,ref_t* result)
{
	RUNTIME_CRITICAL_ENTER();
	for(unsigned int i=0; i<objs.size(); i++)
	{
		unsigned int I = objs.size() - i - 1;
		print_obj(objs[I]);
	}
	RUNTIME_CRITICAL_LEAVE();
	// return
	*result = REF_IS_VOID;
}

void funcaller_t::Sleep(std::vector<object_t*>& objs,ref_t* result)
{
	if( (1==objs.size()) && (INTEGER==objs[0]->get_type()))
	{
		usleep(objs[0]->i);
	}
	// return
	*result = REF_IS_VOID;
}

void funcaller_t::Write(std::vector<object_t*>& objs,ref_t* result)
			throw (runtime_exception_t)
{
	bool rv = false;
	RUNTIME_CRITICAL_ENTER();
	if( (2==objs.size()) && (STRING==objs[1]->get_type()) && (INTEGER==objs[0]->get_type()))
	{
		rv = ArCom_Write(objs[1]->s->c_str(),objs[0]->i);
	}
	RUNTIME_CRITICAL_LEAVE();

	if(false == rv)
	{
		throw runtime_exception_t(std::string("Wrong parameter when call write."));
	}
	// return
	*result = REF_IS_VOID;
}

void funcaller_t::Read(std::vector<object_t*>& objs,ref_t* result)
			throw (runtime_exception_t)
{
	bool rv = false;
	// return
	*result = REF_IS_VOID;
	RUNTIME_CRITICAL_ENTER();
	if( (1==objs.size()) && (STRING==objs[0]->get_type()))
	{
		int value;
		rv = ArCom_Read(objs[0]->s->c_str(),&value);
		if(rv)
		{
			ref_t ref = heap_manager_t::get_instance()->add_object(INTEGER);
			object_t* obj = heap_manager_t::get_instance()->get_object(ref);
			obj->i = value;
			*result = ref;
		}
	}
	RUNTIME_CRITICAL_LEAVE();

	if(false == rv)
	{
		throw runtime_exception_t(std::string("Wrong parameter when call read."));
	}
}

bool funcaller_t::call_buildin(function_call_t* fc,std::vector<object_t*>& objs,ref_t* result)
					throw (runtime_exception_t)
{
	bool rv = true;

	const char* fname = fc->get_name().c_str();
	if(!strcmp(fname,"print"))
	{
		Print(objs,result);
	}
	else if(!strcmp(fname,"sleep"))
	{
		Sleep(objs,result);
	}
	else if(!strcmp(fname,"write"))
	{
		Write(objs,result);
	}
	else if(!strcmp(fname,"read"))
	{
		Read(objs,result);
	}
	else
	{
		rv = false;
	}
	return rv;
}
