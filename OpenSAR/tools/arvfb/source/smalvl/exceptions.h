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
#ifndef _EXCEPTIONS_H
#define _EXCEPTIONS_H

#include <iostream>
#include <stdexcept>

enum EXCEPTION_TYPE
{
	INTERNAL_EXCEPTION,
	NOT_NUMBER,
	NOT_LOGICAL,
	NULL_REF,
	FUNCTION_NOT_DECL,
	FUNCTION_DECL,
	FUNCTION__CALL_INVALID_ARGUMENTS,
	VAR_DECL,
	VAR_NOT_DECL,
	REF_EXIST,
	REF_INVALID,
	INVALID_COVERSION,
	INVALID_RETURN,
	UNCOMPARABLE_TYPES
};

class exception_t
{
protected:
	std::string message;
public:
	exception_t(const std::string &message="Undefined exception")
	{
		this->message = message;
	}
	virtual void print()
	{
		std::cout << "EXCEPTION: " << message << std::endl;
	}

	virtual void set_message(std::string message)
	{
		this->message = message;
	}

	virtual std::string& get_message()
	{
		return message;
	}
};

class runtime_exception_t: public exception_t
{
	EXCEPTION_TYPE type;
public:
	runtime_exception_t(const std::string &message) : exception_t(message)
	{
	}
	runtime_exception_t(const std::string &message, EXCEPTION_TYPE type) : exception_t(message)
	{
		this->type = type;
	}
	runtime_exception_t(EXCEPTION_TYPE type) : exception_t()
	{
		this->type = type;
		switch(type)
		{
			case NOT_NUMBER:
				message = "Value is not a number";
				break;
			case NOT_LOGICAL:
				message = "Value is not logical";
				break;
			case NULL_REF:
				message = "Null regerence exception";
				break;
			case FUNCTION_NOT_DECL:
				message = "Function not declarated in this scope";
				break;
			case FUNCTION__CALL_INVALID_ARGUMENTS:
				message = "Function call invalid arguments";
				break;
			case INVALID_COVERSION:
				message = "Dynamic type conversion error";
				break;
			case REF_INVALID:
				message = "Incorrect reference to heap object";
				break;
			case VAR_NOT_DECL:
				message = "Variable don't declarated early and not initalized. First mentioning.";
				break;
			case INTERNAL_EXCEPTION:
				message = "Runtime internal exception";
				break;
			case VAR_DECL:
				message = "Variable yet decalared early";
				break;
			case REF_EXIST:
				message = "Reference exist";
				break;
			case UNCOMPARABLE_TYPES:
				message = "I can't compare this values";
				break;
			case INVALID_RETURN:
				message = "Invalid Return, not in Function";
				break;
			default:
				message = "Undefined exception, exception code is " +std::to_string(type);
				break;
		}
	}

	virtual EXCEPTION_TYPE getType()
	{
		return type;
	}
};

class nativelib_exception_t: public exception_t
{
};

#endif
