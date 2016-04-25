package chobits.init;


import java.io.File;
import java.net.URL;

import javax.servlet.ServletConfig;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;

import org.jdom.Element;

import cn.ucans.adapter.OrgAdapter;

import chobits.db.DBBuilder;
import chobits.log.Logger;
import chobits.xml.XMLBuilder;

public class ChobitsInitServlet extends HttpServlet {
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	public void destroy() {
		super.destroy(); 
	}
	
	public void init() throws ServletException {

		try{
			String web_path = this.getServletContext().getRealPath("WEB-INF");
			String config_name = this.getInitParameter("ConfigPath");
			ChobitsConfig.VisitURL = this.getInitParameter("VisitURL");
			ChobitsConfig.StoreFilePath = this.getInitParameter("StoreFilePath");
			
			ChobitsConfig.LogFileForderPath = web_path +"/"+config_name;
			ChobitsConfig.TreeXMLSoucePath = web_path.replaceAll("WEB-INF", "") + "TreeXMLSouce";
			ChobitsConfig.TempFileForderPath = web_path.replaceAll("WEB-INF", "") + "TempFile";
			ChobitsConfig.StoreFilePath = web_path.replaceAll("WEB-INF", "") + "StoreFile";
			//System.out.println(ChobitsConfig.TreeXMLSoucePath);
			
			XMLBuilder xmlbuilder = null;
			Element rootElement = null;
			File file = new File(web_path+"/"+config_name+"/chobits-log.xml");
			if(file.exists()){
				xmlbuilder = new XMLBuilder(file);
				rootElement = xmlbuilder.getRootElement();
				Logger.init(rootElement);
			}else{
				throw new Exception("chobits-log.xml not found at path "+web_path+"/"+config_name+"/chobits-log.xml");
			}
			
			file = new File(web_path+"/"+config_name+"/chobits-db.xml");	
			if(file.exists()){		
				xmlbuilder = new XMLBuilder(file);
				rootElement = xmlbuilder.getRootElement();
				DBBuilder.init(rootElement);
			}else{
				throw new Exception("chobits-db.xml not found at path "+web_path+"/"+config_name+"/chobits-db.xml");
			}	
			
			//HappyOrgBuilder orgbuilder = new HappyOrgBuilder(ChobitsConfig.TreeXMLSoucePath);
		}catch(Exception e){
			e.printStackTrace();
		}		
	}
	
}
