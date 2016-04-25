package cn.ucans.servlet;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.PrintWriter;
import java.net.URLDecoder;
import java.net.URLEncoder;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import jxl.Sheet;
import jxl.Workbook;

import org.apache.commons.fileupload.FileItem;
import org.apache.commons.fileupload.disk.DiskFileItemFactory;
import org.apache.commons.fileupload.servlet.ServletFileUpload;

import chobits.db.TransformChinese;
import chobits.init.ChobitsConfig;

public class ExportSendObjectNumberServlet extends HttpServlet {

	/**
	 * 
	 */
	private static final long serialVersionUID = -2480482853066834884L;
	/**
	 * Constructor of the object.
	 */
	public ExportSendObjectNumberServlet() {
		super();
	}

	/**
	 * Destruction of the servlet. <br>
	 */
	public void destroy() {
		super.destroy(); // Just puts "destroy" string in log
		// Put your code here
	}

	/**
	 * The doGet method of the servlet. <br>
	 *
	 * This method is called when a form has its tag value method equals to get.
	 * 
	 * @param request the request send by the client to the server
	 * @param response the response send by the server to the client
	 * @throws ServletException if an error occurred
	 * @throws IOException if an error occurred
	 */
	public void doGet(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		
		request.setCharacterEncoding("GB2312"); 
		response.setContentType("text/html; charset=UTF-8");
		String uploadPath = ChobitsConfig.TempFileForderPath+"/";
		String userid = request.getParameter("userid");
		String org_id = request.getParameter("org_id");
		String sendGroupName ="";
		sendGroupName =  TransformChinese.unescape(request.getParameter("sendGroupName"));
		String language = request.getParameter("language");
		System.out.println(userid+"--"+org_id+"--"+sendGroupName+"--"+language);
		
		DiskFileItemFactory factory = new DiskFileItemFactory();
		factory.setSizeThreshold(4096);
		ServletFileUpload upload = new ServletFileUpload(factory);
		upload.setSizeMax(100 * 1024 * 1024);
		String linkPath = "";
		String name = "";
		String content = "<Item>";
		Connection con = null;
		PreparedStatement ps = null;
		ResultSet rs = null;
		int num = 0;
		int count = 0;
		String tableNameTime = "";
		try {
			List fileItems = upload.parseRequest(request);
			Iterator iter = fileItems.iterator();
			while (iter.hasNext()){
				FileItem item = (FileItem) iter.next();
				if (!item.isFormField()){					
					name = item.getName();
					if(item.getSize()>100 * 1024 * 1024L){
						PrintWriter out = response.getWriter();
						out.write("文件太大，不能超过100M ！");
						return;
					}
					if(name.indexOf(".xls")>-1){
						Date date = new Date();
						SimpleDateFormat sdf = new SimpleDateFormat();
						sdf.applyPattern("yyyyMMddhhmmss");
						String str = sdf.format(date);
						tableNameTime = "T_Info_Number_List_"+sdf.format(date).substring(0, 6);
						String sssionID = request.getSession().getId();
						linkPath = uploadPath + str+ "temp_"+sssionID + name;
						item.write(new File(linkPath));		
					}else{
						PrintWriter out = response.getWriter();
						if(!linkPath.equals("")){
							File file = new File(linkPath);
							if(file.exists()){
								file.delete();
							}
						}
						out.write("上传文件必须为Excel文件！");
						return;
					}		
				}
			}
			
			List PhoneList = this.trackExcel(linkPath);
			if(PhoneList.size()<1){
				PrintWriter out = response.getWriter();
				out.write("您上传的文件是空文件，请确认！");
				if(!linkPath.equals("")){
					File file = new File(linkPath);
					if(file.exists()){
						file.delete();
					}
				}
				return;	
			}
			try{
				chobits.db.DBBuilder builder = new chobits.db.DBBuilder();
				con = builder.getConnection();
				con.setAutoCommit(false);
				long sendGroupID = builder.getOracleSequreNextValue("S_Info");
				for(int i =0;i<PhoneList.size();i++){
					Map map = (Map)PhoneList.get(i);
					String phoneNumber = (String)map.get("phoneCode");
					ps = con.prepareStatement("select count(*) numCount from "+tableNameTime+" where Telephone_Number='"+phoneNumber+"' and Send_Group_ID="+sendGroupID+" ");
					rs = ps.executeQuery();
					while(rs.next()){
						count = rs.getInt("numCount");
						if(count>0){
							con.rollback();
							this.close(con, ps, rs);
							PrintWriter out = response.getWriter();
							i++;
							i++;
							if(!linkPath.equals("")){
								File file = new File(linkPath);
								if(file.exists()){
									file.delete();
								}
							}
							out.write("文件第"+i+"行,终端手机号码："+phoneNumber+" 在文件中有重复,请确认后重新导入！");
							return;	
						}
					}
					ps.close();
					rs.close();
					ps = con.prepareStatement("insert into "+tableNameTime+"(Telephone_Number,Send_Group_ID) values('"+phoneNumber+"',"+sendGroupID+")");
					ps.executeUpdate();
					ps.close();
					num = num+1;
				}
				 
				ps = con.prepareStatement("insert into T_Info_Number_Object(Send_Group_ID,Send_Group_Name,Amount,Language,Mode_Type,User_ID,Org_ID,Insert_Time) "+
						" values("+sendGroupID+",'"+sendGroupName+"',"+num+",'"+language+"','1','"+userid+"','"+org_id+"',sysdate)");
				ps.executeUpdate();
				ps.close();
				con.commit();
				this.close(con, ps, rs);
			}catch (SQLException e){
				con.rollback();
				this.close(con, ps, rs);
				if(e.getErrorCode() == 1){
					PrintWriter out = response.getWriter();
					if(!linkPath.equals("")){
						File file = new File(linkPath);
						if(file.exists()){
							file.delete();
						}
					}
					out.write("发送清单名称已经存在，请重新输入！");
					return;
				}
				PrintWriter out = response.getWriter();
				if(!linkPath.equals("")){
					File file = new File(linkPath);
					if(file.exists()){
						file.delete();
					}
				}
				out.write(e.getMessage());
				return;
			}
		}catch (Exception e){
			PrintWriter out = response.getWriter();
			if(!linkPath.equals("")){
				File file = new File(linkPath);
				if(file.exists()){
					file.delete();
				}
			}
			out.write(e.getMessage());
			return;
		}
		PrintWriter out = response.getWriter();
		out.write("成功导入了<"+num+">个终端手机号码!");
		if(!linkPath.equals("")){
			File file = new File(linkPath);
			if(file.exists()){
				file.delete();
			}
		}
	}

	private List trackExcel(String fileLinkPath)throws Exception{
		Pattern pattern1 = Pattern.compile("^[\\d]{11}$");
		List list = new ArrayList();
		int RowCount = -1;
		int ColumnCount = -1;
		File file = new File(fileLinkPath);
		if(!file.exists()){
			throw new Exception("文件"+file.getName()+"意外丢失！");
		}
		InputStream is = new FileInputStream(file);
		Workbook workbook = Workbook.getWorkbook(is);
		is.close();
		
		int tem_row_count = -1;
		int tem_column_count = -1;
		Sheet sheet = workbook.getSheet("终端手机号码");
		if(!sheet.getName().equals("终端手机号码")||sheet.getColumns()>1){
			
			throw new Exception("Excel文件的格式不正确，请下载模板后在进行导入！");
		}
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
	
		RowCount = tem_row_count;
		ColumnCount = tem_column_count;
		
		for (int i = 1; i <= RowCount - 1; i++){
			String phoneCode = sheet.getCell(0, i).getContents().toString().trim();
			Map map = new HashMap();
			Matcher matcher1 = pattern1.matcher(phoneCode);
			boolean bool = matcher1.matches();
			if(phoneCode==""||phoneCode.length() ==0){
				continue;
			}
			if(bool){
				map.put("phoneCode", phoneCode);
			}else{	
				i++;
				list.clear();
				throw new Exception("文件第"+i+"行,终端手机号码："+phoneCode+"  格式不正确,请确认后重新导入！");
			}
			list.add(map);
		}
		workbook.close();
		return list;
	}
	
	
	
	
	
	/**
	 * The doPost method of the servlet. <br>
	 *
	 * This method is called when a form has its tag value method equals to post.
	 * 
	 * @param request the request send by the client to the server
	 * @param response the response send by the server to the client
	 * @throws ServletException if an error occurred
	 * @throws IOException if an error occurred
	 */
	public void doPost(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {

		this.doGet(request, response);
	}

	/**
	 * Initialization of the servlet. <br>
	 *
	 * @throws ServletException if an error occurs
	 */
	public void init() throws ServletException {
		// Put your code here
	}
	public String trackValue(Object object){
		if(object == null || object.toString().trim().length()==0 || object.toString().trim().equals("null") || object.toString().trim().equals("NULL")){
			return "";
		}else{
			return object.toString();
		}
	}
	private void close(Connection connection,PreparedStatement statement,ResultSet resultset){
		try{
			if(resultset != null){
				resultset.close();
			}
			if(statement != null){
				statement.close();
			}
			if(connection != null){
				connection.close();
			}
		}catch(Exception e){}		
	}
}
