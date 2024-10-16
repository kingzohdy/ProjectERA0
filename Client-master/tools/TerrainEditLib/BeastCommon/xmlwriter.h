/** \file 
* XML writer helper
*/ 
#ifndef XMLWRITER_H
#define XMLWRITER_H

#include <beastapi/beastapitypes.h>

namespace bex {
	typedef std::basic_string<TCHAR> tstring;

	class XMLWriter {
	private:
		static const unsigned int m_tabsize = 2;

	public:

		class ScopedTag {
		public:
			ScopedTag(XMLWriter& xml, const tstring& name) : m_xml(xml) {
				m_xml.tag(name);
			}
			~ScopedTag() {
				m_xml.endtag();
			}
		private:
			XMLWriter& m_xml;
		};

		XMLWriter(std::ostream& output) : 
			m_output(output),
			m_tab(0) {
			m_output << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>" << std::endl;
		}
		
		~XMLWriter() {
			while(endtag());
		}

		void tag(const tstring& name, bool inl = false) {
			m_output << tab() << "<" << name << ">";
			if (!inl) {
				 m_output << std::endl;
			}
			m_tags.push_back(name);
			++m_tab;
		}

		bool endtag(bool inl = false) {
			if (m_tags.empty()) {
				return false;
			}
			--m_tab;
			if (!inl) {
				m_output << tab();
			}
			m_output << "</" << m_tags.back() << ">" << std::endl;
			m_tags.pop_back();
			return true;
		}

		template<class T>
		void data(const tstring& name, const T& value) {
			tag(name, true);
			m_output << value;
			endtag(true);
		}

		void data(const tstring& name, bool value) {
			tag(name, true);
			m_output << (value?_T("true"):_T("false"));
			endtag(true);
		}

	private:
		std::ostream& m_output;
		std::vector<tstring> m_tags; 
		unsigned int m_tab;

		std::string tab() {
			return std::string(m_tabsize * m_tab, ' ');
		}
	};

	template<>
	inline void XMLWriter::data<ILBLinearRGB>(const tstring& name, const ILBLinearRGB& value) {
	        tag(name);
		data(_T("r"), value.r);
		data(_T("g"), value.g);
		data(_T("b"), value.b);
		endtag();
	}

	template<>
	inline void XMLWriter::data<ILBLinearRGBA>(const tstring& name, const ILBLinearRGBA& value) {
	        tag(name);
		data(_T("r"), value.r);
		data(_T("g"), value.g);
		data(_T("b"), value.b);
		data(_T("a"), value.a);
		endtag();
	}

	template<>
	inline void XMLWriter::data<ILBVec2>(const tstring& name, const ILBVec2& value) {
	        tag(name);
		data(_T("x"), value.x);
		data(_T("y"), value.y);
		endtag();
	}



	class ScopedTag {
	public:
		ScopedTag(XMLWriter& xml, const tstring& name) :
			m_xml(xml) {
			m_xml.tag(name);
		}
		~ScopedTag() {
			m_xml.endtag();
		}
	private:
		XMLWriter& m_xml;
	};

}


#endif//XMLWRITER_H
