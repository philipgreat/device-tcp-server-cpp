package cn.ucans.servlet;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Date;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Vector;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import jxl.Sheet;
import jxl.Workbook;

import org.apache.commons.fileupload.FileItem;
import org.apache.commons.fileupload.disk.DiskFileItemFactory;
import org.apache.commons.fileupload.servlet.ServletFileUpload;
import org.jdom.Element;

import cn.ucans.bean.T_TERMINAL_INFO;

import chobits.common.DateUtil;
import chobits.common.RegularExpression;
import chobits.db.DBBuilder;
import chobits.db.DBFactory;
import chobits.init.ChobitsConfig;
import chobits.log.Logger;
import chobits.xml.XMLBuilder;

public class DocumentFileUploadServlet extends HttpServlet {

	/**
	 * 
	 */
	private static final long serialVersionUID = -4009219607342858029L;
	
	private Logger logger = new Logger(DocumentFileUploadServlet.class.getName());

	public void doGet(HttpServletRequest request, HttpServletResponse response)throws ServletException, IOException {
		String xml_result = "";
		try{
			String org_id = request.getParameter("Org_ID");
			response.setContentType("text/html; charset=UTF-8");
			
			UploadFileResult sr = this.storeFile(request, org_id);
			if(sr.Servlet_Error_Code != 0){
				throw new Exception(sr.Servlet_Error_Message);
			}
			xml_result = this.getReturnXML(0, null, sr.link);
		}catch(Exception e){
			xml_result = this.getReturnXML(-1, e.getMessage(), null);
		}
		
		PrintWriter out = response.getWriter();
		out.write(xml_result);
		out.close();
	}
	
	private UploadFileResult storeFile(HttpServletRequest request,String org_id){
		UploadFileResult sr = new UploadFileResult();
		DateUtil du = new DateUtil();
		String timeformat = du.getYMDHMSTime(DateUtil.Format_EN_yyyyMMddHHmmss);
		String sssionID = request.getSession().getId();
		String linkPath = "";
		try{
			DiskFileItemFactory factory = new DiskFileItemFactory();
			factory.setSizeThreshold(4096);
			ServletFileUpload upload = new ServletFileUpload(factory);
			upload.setSizeMax(100 * 1024 * 1024);
			String name = "";			
		
			List fileItems = upload.parseRequest(request);
			Iterator iter = fileItems.iterator();
			while (iter.hasNext()){
				FileItem item = (FileItem) iter.next();
				if (!item.isFormField()){					
					name = item.getName();
					if(item.getSize()>2 * 1024 * 1024L){
						throw new Exception("文件太大，不能超过2M ！");
					}

					File storeForder = new File(ChobitsConfig.StoreFilePath);
					if(!storeForder.exists()){
						storeForder.mkdirs();
					}
					int inde_point = name.lastIndexOf(".");
					String extend_name = name.substring(inde_point, name.length());
					linkPath = ChobitsConfig.StoreFilePath + "/" + sssionID + "_" + org_id + "_" + timeformat + "_" + extend_name;
					sr.link = linkPath;
					File file_item = new File(linkPath);
					if(file_item.exists()){
						file_item.delete();
					}
					item.write(new File(linkPath));			
				}
			}
		}catch(Exception e){
			logger.error("上传文件出错，原因："+e.getMessage());
			sr.Servlet_Error_Code = -200;
			sr.Servlet_Error_Message = "上传文件出错，原因："+e.getMessage();
		}
		return sr;
	}
	
	private String getReturnXML(int code, String message, String url){
		String xml = "<Result code=\""+code+"\" message=\""+message+"\" url=\""+url+"\"></Result>";
		return xml;
	}
	
	public void doPost(HttpServletRequest request, HttpServletResponse response)throws ServletException, IOException {
		doGet(request, response);
	}

	public void init() throws ServletException {
		// Put your code here
	}

}



