package chobits.xml;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.jdom.Document;
import org.jdom.Element;
import org.jdom.JDOMException;
import org.jdom.input.SAXBuilder;
import org.jdom.output.XMLOutputter;
import org.xml.sax.InputSource;

import cn.ucans.adapter.OrgAdapter;

import chobits.common.FileUtil;
import chobits.log.Logger;

public class XMLBuilder {
	
	private Logger logger = new Logger(XMLBuilder.class.getName());
	
	public static int ADD_FROM_FORMAT = 1;
	public static int ADD_BEFORE_CLEAR = 2;
	
	public static int INIT_WITH_HEAD = 1;
	public static int INIT_WITHOUT_HEAD = 2;
	
	private Document document = null;
	private Element rootElement = null;
	
	public XMLBuilder(){}
	
	public XMLBuilder(File file){
		try{
			SAXBuilder builder = new SAXBuilder();
			this.document = builder.build(file);
			this.rootElement = this.document.getRootElement();
			
		}catch(Exception e){
			logger.error(e.getMessage());
		}
	}
	
	public void setRootElement(Element root_element){
		this.rootElement = root_element;
	}
	
	public XMLBuilder(String xmlString,int type){
		try{
			if(xmlString == null || xmlString.trim().length() == 0){
				xmlString = "<?xml version='1.0' encoding='UTF-8'?>";
			}
			StringReader sr = null;
			if(type == XMLBuilder.INIT_WITH_HEAD){
				sr = new StringReader(xmlString);
			}else {
				sr = new StringReader("<?xml version='1.0' encoding='UTF-8'?>"+xmlString);
			}
			InputSource source = new InputSource(sr);
	        SAXBuilder sb = new SAXBuilder();
	        document = sb.build(source);
	        rootElement = document.getRootElement();
	        sr.close();	
		}catch(Exception e){
			logger.error(e.getMessage());
		}
	}
	
	public void setRootElement(String title, String value){
		Element element = new Element(title);
		element.setText(value);
		this.document.setRootElement(element);
	}
	
	public Element getRootElement(){
		return this.rootElement;
	}
	
	public String toXML(){
		String text = null;
		XMLOutputter outputer = new XMLOutputter();
		//outputer.setEncoding("UTF-8");
		text = outputer.outputString(this.document);
		return text;
	}
	
	public String toXML(Document document){
		String text = null;
		XMLOutputter outputer = new XMLOutputter();
		//outputer.setEncoding("UTF-8");
		text = outputer.outputString(document);
		return text;
	}
	
	public String toXML(Element element){
		String text = null;
		XMLOutputter outputer = new XMLOutputter();
		//outputer.setEncoding("UTF-8");
		text = outputer.outputString(element);
		return text;
	}
	
	public String toString(){
		return this.document.toString();
	}
}
