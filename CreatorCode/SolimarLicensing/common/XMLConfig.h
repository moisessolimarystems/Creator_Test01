#ifndef __XML_Config_254879240975470943_h__
#define __XML_Config_254879240975470943_h__


#import <msxml4.dll>


class XMLConfig
{
public:
	
	//typedef IXMLDOMNodePtr XMLConfigNodePtr;
	
	/** Parases an xml document */
	XMLConfig();
	virtual ~XMLConfig();
	
	HRESULT Load(_bstr_t XMLPathOrURL);
	HRESULT Load(IStream* pIStream);
	HRESULT GetRoot(/*[out]*/ MSXML2::IXMLDOMNodePtr &root);

	HRESULT GetChild(MSXML2::IXMLDOMNodePtr node, _bstr_t childname, /*[out]*/ MSXML2::IXMLDOMNodePtr &childnode);
	HRESULT GetChildByIndex(MSXML2::IXMLDOMNodePtr node, int childindex, /*[out]*/ MSXML2::IXMLDOMNodePtr &childnode);
	HRESULT GetChildByElement(MSXML2::IXMLDOMNodePtr node, _bstr_t elementNodeName, /*[out]*/ MSXML2::IXMLDOMNodePtr &childnode);
	HRESULT GetChildCount(MSXML2::IXMLDOMNodePtr node, /*[out]*/ int &count);
	
	HRESULT GetValue(MSXML2::IXMLDOMNodePtr node, /*[out]*/ _bstr_t &value);
	HRESULT GetAttribute(MSXML2::IXMLDOMNodePtr node, _bstr_t attribute, /*[out]*/ _bstr_t &value);
	HRESULT GetXML(MSXML2::IXMLDOMNodePtr node, /*[out]*/ _bstr_t &value);
	HRESULT GetPath(MSXML2::IXMLDOMNodePtr node, _bstr_t path, /*[out]*/ MSXML2::IXMLDOMNodePtr &childnode);
	HRESULT GetPathValue(MSXML2::IXMLDOMNodePtr node, _bstr_t path, /*[out]*/ _bstr_t &value);
	HRESULT GetPathXML(MSXML2::IXMLDOMNodePtr node, _bstr_t path, /*[out]*/ _bstr_t &value);
	
protected:
	MSXML2::IXMLDOMNodePtr m_root;
	MSXML2::IXMLDOMDocumentPtr m_xmldoc;
};

#endif
