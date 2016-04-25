package cn.ucans.servlet;

public class UploadFileResult {
	public int Servlet_Error_Code = 0;
	public String Servlet_Error_Message = "";
	public String link = "";
	public String sql = "";
	public String fileName = "";
	public String fileType = "";
	
	public void clear(){
		this.Servlet_Error_Code = 0;
		this.Servlet_Error_Message = "";
		this.link = "";
		this.sql = "";
	}
}
