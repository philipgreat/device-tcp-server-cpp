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

import chobits.common.RegularExpression;
import chobits.db.DBBuilder;
import chobits.init.ChobitsConfig;
import chobits.xml.XMLBuilder;

public class TerminalExcelFileUploadServlet extends HttpServlet {

	/**
	 * 
	 */
	private static final long serialVersionUID = -4009219607342858029L;
	
	private DBBuilder builder = new DBBuilder();

	public void doGet(HttpServletRequest request, HttpServletResponse response)throws ServletException, IOException {
		String uploadPath = ChobitsConfig.TempFileForderPath;
		
		String user_id = request.getParameter("UserID");
		response.setContentType("text/html; charset=UTF-8");
		DiskFileItemFactory factory = new DiskFileItemFactory();
		factory.setSizeThreshold(4096);
		ServletFileUpload upload = new ServletFileUpload(factory);
		upload.setSizeMax(100 * 1024 * 1024);
		String linkPath = "";
		String name = "";
		
		String xml_result = "";
		int code = -1;
		String message = "";
		List<T_TERMINAL_INFO> list_error = new ArrayList<T_TERMINAL_INFO>();
		try {
			List fileItems = upload.parseRequest(request);
			Iterator iter = fileItems.iterator();
			while (iter.hasNext()){
				FileItem item = (FileItem) iter.next();
				if (!item.isFormField()){					
					name = item.getName();
					if(item.getSize()>100 * 1024 * 1024L){
						throw new Exception("文件太大，不能超过100M ！");
					}
					if(name.indexOf(".xls")>-1){
						Date date = new Date();
						String str = date.toLocaleString();
						str = str.replaceAll(" ", "");
						str = str.replaceAll("-", "");
						str = str.replaceAll(":", "");
						String sssionID = request.getSession().getId();
						linkPath = uploadPath + str+ "upload_"+sssionID + name;
						item.write(new File(linkPath));			
					}else{
						throw new Exception("上传文件必须为Excel文件！");
					}		
				}
			}
			
			List<T_TERMINAL_INFO> list = this.trackExcel(linkPath);
			String error_message = "";
			boolean dook = true;			
			list_error = this.checkDataIsNull(list);
			if(list_error.size()>0){
				error_message = "Excel文件中某些数据为空，";
				dook = false;
			}
			
			if(dook){
				list_error = this.checkDataISNumber(list);
				if(list_error.size()>0){
					error_message = "指定的列所存数据不是数字，";
					dook = false;
				}
			}
			
			if(dook){
				list_error = this.checkDataLength(list);
				if(list_error.size()>0){
					error_message = "指定的列所存数据长度不对，";
					dook = false;
				}
			}
			
			if(dook){
				list_error = this.checkIMEI_NUMBER_UIM_NUMBER_IsExist(list);
				if(list_error.size()>0){
					error_message = "IMEI编号和UIM编号已经被录入过，";
					dook = false;
				}
			}
			
			if(dook){
				list_error = this.checkTerminalType_OrgID_IsExist(list);
				if(list_error.size()>0){
					error_message = "终端厂商和终端类型不对应，";
					dook = false;
				}
			}
			
			if(dook){
				this.lead(list, user_id);
				xml_result = this.getReturnXML(-1, "数据全部导入成功！", null);
			}else{
				xml_result = this.getReturnXML(-1, error_message+"，导入操作未执行！", list_error);
			}			
		}catch (Exception e){
			xml_result = this.getReturnXML(-2, "导入数据出错，原因："+e.getMessage(), null);
			if(!linkPath.equals("")){
				File file = new File(linkPath);
				if(file.exists()){
					file.delete();
				}
			}
		}
		PrintWriter out = response.getWriter();
		out.write(xml_result);
		out.close();
	}
	
	private String getReturnXML(int code, String message, List<T_TERMINAL_INFO> list){
		String xml = "<Result code=\""+code+"\" message=\""+message+"\"></Result>";
		XMLBuilder xb = new XMLBuilder(xml,XMLBuilder.INIT_WITHOUT_HEAD);
		Element root_element = xb.getRootElement();
		if(list != null && list.size()>0){
			for(int i=0;i<list.size();i++){
				T_TERMINAL_INFO terminal = list.get(i);
				Element element = new Element("row");
				element.setAttribute("IMEI_NUMBER", terminal.IMEI_NUMBER);
				element.setAttribute("UIM_NUMBER", terminal.UIM_NUMBER);
				element.setAttribute("MSISDN", terminal.MSISDN);
				element.setAttribute("PRODUCT_ORG_ID", terminal.PRODUCT_ORG_ID);
				element.setAttribute("TERMINAL_TYPE", terminal.TERMINAL_TYPE);
				
				root_element.addContent(element);			
			}
		}	
		
		return xb.toXML(root_element);
	}
	
	private List<T_TERMINAL_INFO> trackExcel(String fileLinkPath)throws Exception{
		
		File file = new File(fileLinkPath);
		if(!file.exists()){
			throw new Exception("文件"+fileLinkPath+"意外丢失！");
		}
		InputStream is = new FileInputStream(file);
		Workbook workbook = Workbook.getWorkbook(is);
		is.close();
		
		int tem_row_count = -1;
		int tem_column_count = -1;
		
		Sheet sheet = workbook.getSheet("Terminal");
		tem_row_count = sheet.getRows();
		tem_column_count = sheet.getColumns();
		for (int r = tem_row_count; r > 0; r--){
			String cellvalue = sheet.getCell(0, r - 1).getContents().toString().trim();
			if (cellvalue.equals("") || cellvalue == null)
				tem_row_count--;
		}
	
		for (int c = tem_column_count; c > 0; c--){
			String cellvalue = sheet.getCell(c - 1, 0).getContents().toString().trim();
			if (cellvalue.equals("") || cellvalue == null)
				tem_column_count--;
		}
	
		int RowCount = tem_row_count;
//		int ColumnCount = tem_column_count;
		
		List<T_TERMINAL_INFO> list = new ArrayList<T_TERMINAL_INFO>();
		for (int i = 1; i <= RowCount - 1; i++){
			T_TERMINAL_INFO terminal = new T_TERMINAL_INFO();
			terminal.IMEI_NUMBER = sheet.getCell(0, i).getContents().toString().trim();
			terminal.UIM_NUMBER = sheet.getCell(1, i).getContents().toString().trim();
			terminal.MSISDN = sheet.getCell(2, i).getContents().toString().trim();
			terminal.CONTENT_VERSION = sheet.getCell(3, i).getContents().toString().trim();
			terminal.PRODUCT_ORG_ID = sheet.getCell(4, i).getContents().toString().trim();
			terminal.TERMINAL_TYPE = sheet.getCell(5, i).getContents().toString().trim();
			terminal.PRODUCT_DATE = sheet.getCell(6, i).getContents().toString().trim();
			terminal.MAINTAIN_LIMITED_DATE = sheet.getCell(7, i).getContents().toString().trim();
			list.add(terminal);			
		}
	
		workbook.close();
		return list;
	}
	
	private List<T_TERMINAL_INFO> checkDataIsNull(List<T_TERMINAL_INFO> list){
		List<T_TERMINAL_INFO> list_error = new ArrayList<T_TERMINAL_INFO>();
		for(int i=0;i<list.size();i++){
			boolean dook = true;
			T_TERMINAL_INFO terminal = list.get(i);
			if(terminal.IMEI_NUMBER == null || terminal.IMEI_NUMBER.equals("")){
				dook = false;
			}
			if(dook){
				if(terminal.UIM_NUMBER == null || terminal.UIM_NUMBER.equals("")){
					dook = false;
				}
			}
			if(dook){
				if(terminal.MSISDN == null || terminal.MSISDN.equals("")){
					dook = false;
				}
			}
			if(dook){
				if(terminal.PRODUCT_ORG_ID == null || terminal.PRODUCT_ORG_ID.equals("")){
					dook = false;
				}
			}
			if(dook){
				if(terminal.TERMINAL_TYPE == null || terminal.TERMINAL_TYPE.equals("")){
					dook = false;
				}	
			}
			if(!dook){
				list_error.add(terminal);
			}		
		}
		return list_error;
	}
	
	private List<T_TERMINAL_INFO> checkDataLength(List<T_TERMINAL_INFO> list){
		List<T_TERMINAL_INFO> list_error = new ArrayList<T_TERMINAL_INFO>();
		for(int i=0;i<list.size();i++){
			boolean dook = true;
			T_TERMINAL_INFO terminal = list.get(i);
			if(!(terminal.IMEI_NUMBER.length() == 15)){
				dook = false;
			}
			if(dook){
				if(!(terminal.UIM_NUMBER.length() == 15)){
					dook = false;
				}
			}
			if(dook){
				if(!(terminal.MSISDN.length() == 11)){
					dook = false;
				}
			}
			if(!dook){
				list_error.add(terminal);
			}	
		}
		return list_error;
	}
	
	private List<T_TERMINAL_INFO> checkDataISNumber(List<T_TERMINAL_INFO> list){
		List<T_TERMINAL_INFO> list_error = new ArrayList<T_TERMINAL_INFO>();
		RegularExpression regular = new RegularExpression();
		for(int i=0;i<list.size();i++){
			boolean dook = true;
			T_TERMINAL_INFO terminal = list.get(i);
			if(!regular.isNumeric(terminal.IMEI_NUMBER)){
				dook = false;
			}
			if(dook){
				if(!regular.isNumeric(terminal.UIM_NUMBER)){
					dook = false;
				}
			}
			if(dook){
				if(!regular.isNumeric(terminal.MSISDN)){
					dook = false;
				}
			}
			if(!dook){
				list_error.add(terminal);
			}	
		}
		return list_error;
	}
	
	private List<T_TERMINAL_INFO> checkIMEI_NUMBER_UIM_NUMBER_IsExist(List<T_TERMINAL_INFO> list){
		List<T_TERMINAL_INFO> list_error = new ArrayList<T_TERMINAL_INFO>();
		for(int i=0;i<list.size();i++){
			T_TERMINAL_INFO terminal = list.get(i);
			int count = builder.getCount("T_Terminal_Info", "IMEI_NUMBER='"+terminal.IMEI_NUMBER+"' and UIM_NUMBER='"+terminal.UIM_NUMBER+"'");
			if(count>0){
				list_error.add(terminal);
			}
		}
		return list_error;
	}
	
	private List<T_TERMINAL_INFO> checkTerminalType_OrgID_IsExist(List<T_TERMINAL_INFO> list){
		List<T_TERMINAL_INFO> list_error = new ArrayList<T_TERMINAL_INFO>();
		for(int i=0;i<list.size();i++){
			T_TERMINAL_INFO terminal = list.get(i);
			int count = builder.getCount("T_Terminal_Type", "Org_ID='"+terminal.PRODUCT_ORG_ID+"' and Terminal_Type='"+terminal.TERMINAL_TYPE+"'");
			if(count==0){
				list_error.add(terminal);
			}
		}
		return list_error;
	}
	
	private void lead(List<T_TERMINAL_INFO> list,String user_id) throws Exception{
		List<String> list_sql = new ArrayList<String>();
		for(int i=0;i<list.size();i++){
			T_TERMINAL_INFO terminal = list.get(i);
			String sql_insert = "insert into T_TERMINAL_INFO (IMEI_NUMBER,UIM_NUMBER,MSISDN,PRODUCT_ORG_ID,TERMINAL_TYPE,PRODUCT_DATE,MAINTAIN_LIMITED_DATE,CONTENT_VERSION,Status,Join_Time,Join_User_ID) values ";
			sql_insert += "('"+terminal.IMEI_NUMBER+"','"+terminal.UIM_NUMBER+"','"+terminal.MSISDN+"','"+terminal.PRODUCT_ORG_ID+"','"+terminal.TERMINAL_TYPE+"','"+terminal.PRODUCT_DATE+"','"+terminal.MAINTAIN_LIMITED_DATE+"','"+terminal.CONTENT_VERSION+"','0',sysdate,'"+user_id+"')";
			String sql_insert_his = "insert into T_Terminal_Change_His(Imei_Number,Uim_Number,Msisdn,Change_Type,User_ID,Change_Time) values ";
			sql_insert_his += "('"+terminal.IMEI_NUMBER+"','"+terminal.UIM_NUMBER+"','"+terminal.MSISDN+"','01','"+user_id+"',sysdate);";
			list_sql.add(sql_insert);
			list_sql.add(sql_insert_his);
		}
		builder.excute(list_sql);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("终端管理，批量导入终端信息出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
	}

	public void doPost(HttpServletRequest request, HttpServletResponse response)throws ServletException, IOException {
		doGet(request, response);
	}

	public void init() throws ServletException {
		// Put your code here
	}

}



