#include "common.h"

gtString gost::gtCVarSystem_printDescription( const gtCVarSystemNode& node ){

	auto CVS = gtMainSystem::getInstance()->getCVarSystem();

	gtString result;

	gtArray<gtString> strArray;
	util::stringGetWords<char16_t>( &strArray, node.commandLine );
	u32 sz = strArray.size();
	if( sz > gtConst1U ){
		CVS->printDescription( strArray[ gtConst1U ] );
	}else{
		CVS->printDescription( node.command );
	}

	return result;
}

gtCVarSystemImpl::gtCVarSystemImpl(){}
gtCVarSystemImpl::~gtCVarSystemImpl(){}

void gtCVarSystemImpl::printDescription( const gtString& command ){
	for( auto& item : m_commands ){
		if( command == item.command ){
			gtLogWriter::printInfo( u"`%s` - %s", item.command.data(), item.description.data() );
			return;
		}
	}
	gtLogWriter::printWarning( u"Unknown command `%s`", command.data() );
}

gtString gtCVarSystemImpl::processCommand( const gtString& cmd ){

	gtLogWriter::printInfo( u">>> %s", cmd.data() );

	gtString result;

	if( cmd.size() ){
		gtString command = util::stringGetFirstWord( cmd );
		for( auto& item : m_commands ){
			if( command == item.command ){
				if( item.func ){
					item.commandLine = cmd;
					result = item.func( item );
				}
				return result;
			}

		}

		gtLogWriter::printWarning( u"Unknown command `%s`", command.data() );
	}

	return result;
}

bool gtCVarSystemImpl::isHave( const gtString& command ){
	for( const auto& i : m_commands ){
		if( i.command == command ) return true;
	}
	return false;
}

bool gtCVarSystemImpl::addCommand( gtCVarType type, const gtString& command, const gtString& description, const gtString& parameters, gtString(*func)(const gtCVarSystemNode&) ){
	if( isHave( command ) ){
		gtLogWriter::printWarning( u"Can not add command \"%s\" to CVarSystem", command.data() );
		return false;
	}
	m_commands.push_back( gtCVarSystemNode( type, command, description, parameters, func ) );
	return true;
}

bool gtCVarSystemImpl::removeCommand( const gtString& command ){
	if( isHave( command ) ){
		u32 index = gtConst0U;
		for( const auto& i : m_commands ){
			if( i.command == command ){
				m_commands.erase( index );
				return true;
			}
			++index;
		}
	}
	gtLogWriter::printWarning( u"Can not remove command \"%s\" from CVarSystem", command.data() );
	return false;
}

/*
Copyright (c) 2017-2018 532235

Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
and associated documentation files (the "Software"), to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT 
LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/