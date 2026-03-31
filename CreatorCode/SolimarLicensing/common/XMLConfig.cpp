/*
#include "..\headers\XMLConfig.h"


#include "..\headers\ss_debug.h"
#include "..\headers\ss_error.h"
*/

#include "XMLConfig.h"

//void TRACE(LPCTSTR pFormat, ...)
//{
//}

/** Parases an xml document */
XMLConfig::XMLConfig() : m_root(NULL), m_xmldoc(NULL)
{
	;
}

XMLConfig::~XMLConfig()
{
	;
}

//protected: IXMLDOMNodePtr m_root;

HRESULT XMLConfig::Load(_bstr_t XMLPathOrURL)
{
	HRESULT hr=S_OK;
	
	hr = m_xmldoc.CreateInstance("Msxml.DOMDocument");
	if (SUCCEEDED(hr))
	{
		m_xmldoc->async=false;
		if (m_xmldoc->load(_variant_t(XMLPathOrURL)))
		{
			m_root=m_xmldoc->documentElement;
		}
		else
		{
			wchar_t tmpbuf[1024];
			swprintf_s(tmpbuf, 1024, L"XMLConfig::Load - Cannot load xml document %s, Reason: %d -- %s", (wchar_t*)XMLPathOrURL, m_xmldoc->parseError->errorCode, (wchar_t*)m_xmldoc->parseError->reason);
			OutputDebugString(tmpbuf);
			hr = E_FAIL;
		}
	}

	return hr;
}

HRESULT XMLConfig::Load(IStream* pIStream)
{
	HRESULT hr=S_OK;
	
	hr = m_xmldoc.CreateInstance("Msxml.DOMDocument");
	if (SUCCEEDED(hr))
	{
		m_xmldoc->async=false;
		if (m_xmldoc->load(_variant_t(pIStream)))
		{
			m_root=m_xmldoc->documentElement;
		}
		else
		{
			wchar_t tmpbuf[1024];
			swprintf_s(tmpbuf, 1024, L"XMLConfig::Load - Cannot load xml document from stream, Reason: %d -- %s", m_xmldoc->parseError->errorCode, (wchar_t*)m_xmldoc->parseError->reason);
			OutputDebugString(tmpbuf);
			hr = E_FAIL;
		}
	}

	return hr;
}

HRESULT XMLConfig::GetRoot(/*[out]*/ MSXML2::IXMLDOMNodePtr &root)
{
	HRESULT hr=S_OK;
	
	root=m_root;
	
	return hr;
}

HRESULT XMLConfig::GetChild(MSXML2::IXMLDOMNodePtr node, _bstr_t childname, /*[out]*/ MSXML2::IXMLDOMNodePtr &childnode)
{
	HRESULT hr=HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);

	// default/error return value
	childnode=NULL;

	//::DOMNodeType nodetype;
	MSXML2::DOMNodeType nodetype;
	
	for (int i=0; i<node->childNodes->length && node->childNodes->length>=0; ++i)
	{
		// locate the "name" attribute
		try
		{
			node->childNodes->item[i]->get_nodeType(&nodetype);
			if (nodetype==MSXML2::NODE_ELEMENT)
			{
				_bstr_t nodename=node->childNodes->item[i]->attributes->getNamedItem(L"name")->text;
				// if the name attribute matches the requested name
				if (nodename==childname)
				{
					childnode=node->childNodes->item[i];
					hr = S_OK;
					break;
				}
			}
		}
		// trying to get the text property on a missing "name" attribute will throw exceptions, as will item[i] not existing, etc.
		catch (_com_error &e)
		{

			wchar_t tmpbuf[1024];
			swprintf_s(tmpbuf, 1024, L"XMLConfig::GetChild - 0x%08x", e.Error());
			OutputDebugString(tmpbuf);
			//TRACEhr(e.Error(),_T("XMLConfig::GetChild  --  Probably Ignorable Error"));
		}
	}

	return hr;
}

HRESULT XMLConfig::GetChildByElement(MSXML2::IXMLDOMNodePtr node, _bstr_t elementNodeName, /*[out]*/ MSXML2::IXMLDOMNodePtr &childnode)
{
	HRESULT hr=HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);

	// default/error return value
	childnode=NULL;

	//::DOMNodeType nodetype;
	MSXML2::DOMNodeType nodetype;
	
	for (int i=0; i<node->childNodes->length && node->childNodes->length>=0; ++i)
	{
		// locate the element name
		node->childNodes->item[i]->get_nodeType(&nodetype);
		if (nodetype==MSXML2::NODE_ELEMENT)
		{
			BSTR childElementName;
			node->childNodes->item[i]->get_nodeName(&childElementName);
			// if the element matches the requested name
			if (_wcsicmp(elementNodeName, childElementName) == 0)
			{
				childnode=node->childNodes->item[i];
				hr = S_OK;
				break;
			}
		}
	}

	return hr;
}

HRESULT XMLConfig::GetChildByIndex(MSXML2::IXMLDOMNodePtr node, int childindex, /*[out]*/ MSXML2::IXMLDOMNodePtr &childnode)
{
	HRESULT hr=HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
	
	// default/error return value
	childnode=NULL;
	
	MSXML2::IXMLDOMNodePtr cnode;
	MSXML2::DOMNodeType nodetype;
	
	// loop through, finding the childindex-th option tag
	int cindex=0,index=0;
	while(index<node->childNodes->length && cindex<=childindex)
	{
		// locate the "name" attribute
		cnode=node->childNodes->item[index];

		cnode->get_nodeType(&nodetype);
		if (nodetype==MSXML2::NODE_ELEMENT)
		{
			if (childindex==cindex)
			{
				childnode=cnode;
				hr = S_OK;
				break;
			}
			cindex++;
		}
		index++;
	}
	
	return hr;
}

HRESULT XMLConfig::GetChildCount(MSXML2::IXMLDOMNodePtr node, /*[out]*/ int &count)
{
	HRESULT hr=S_OK;
	
	MSXML2::IXMLDOMNodePtr cnode;
	MSXML2::DOMNodeType nodetype;
	
	// loop through, finding the childindex-th option tag
	int index=0;
	count=0;
	while(index<node->childNodes->length)
	{
		cnode=node->childNodes->item[index];
		cnode->get_nodeType(&nodetype);
		if (nodetype==MSXML2::NODE_ELEMENT)
		{
			count++;
		}
		index++;
	}
	
	return hr;
}


HRESULT XMLConfig::GetValue(MSXML2::IXMLDOMNodePtr node, /*[out]*/ _bstr_t &value)
{
	HRESULT hr=S_OK;
	
	try
	{
		value=node->text;
	}
	catch(_com_error &e)
	{
		return e.Error();
	}
	
	return hr;
}

HRESULT XMLConfig::GetXML(MSXML2::IXMLDOMNodePtr node, /*[out]*/ _bstr_t &value)
{
	HRESULT hr=S_OK;
	
	try
	{
		value=node->xml;
	}
	catch(_com_error &e)
	{
		return e.Error();
	}
	
	return hr;
}

HRESULT XMLConfig::GetPath(MSXML2::IXMLDOMNodePtr node, _bstr_t path, /*[out]*/ MSXML2::IXMLDOMNodePtr &childnode)
{
	HRESULT hr=S_OK;
	
	wchar_t *pBegin, *pStart, *pEnd, *pBuffer;
	
	childnode=node;
	
	// iteratively get children while parsing the path
	pStart=path;
	pEnd=pStart;
	pBegin=pStart;
	while(SUCCEEDED(hr) && pEnd<pStart+path.length())
	{
		// find the end of the path subsection
		while(*pEnd!=L'.' && *pEnd!=L'\0')
			++pEnd;
		
		// get the child with the specified name
		pBuffer=new wchar_t[pEnd-pBegin+1];
		wcsncpy_s(pBuffer, pEnd-pBegin+1, pBegin, pEnd-pBegin);
		pBuffer[pEnd-pBegin]=L'\0';
		hr=GetChild(childnode, _bstr_t(pBuffer), childnode);
		delete [] pBuffer;
		
		pBegin=pEnd+1;
		pEnd=pBegin;
	}
	
	return hr;
}

HRESULT XMLConfig::GetPathValue(MSXML2::IXMLDOMNodePtr node, _bstr_t path, /*[out]*/ _bstr_t &value)
{
	HRESULT hr=S_OK;
	
	MSXML2::IXMLDOMNodePtr tmpnode;
	hr = GetPath(node, path, tmpnode);
	if (SUCCEEDED(hr))
	{
		hr = GetValue(tmpnode, value);
	}
	
	return hr;
}

HRESULT XMLConfig::GetPathXML(MSXML2::IXMLDOMNodePtr node, _bstr_t path, /*[out]*/ _bstr_t &value)
{
	HRESULT hr=S_OK;
	
	MSXML2::IXMLDOMNodePtr tmpnode;
	hr = GetPath(node, path, tmpnode);
	if (SUCCEEDED(hr))
	{
		hr = GetXML(tmpnode, value);
	}
	
	return hr;
}

HRESULT XMLConfig::GetAttribute(MSXML2::IXMLDOMNodePtr node, _bstr_t attribute, /*[out]*/ _bstr_t &value)
{
	HRESULT hr=S_OK;
	
	try
	{
		value=node->attributes->getNamedItem(attribute)->text;
	}
	catch(_com_error &e)
	{
		return e.Error();
	}
	
	return hr;
}
