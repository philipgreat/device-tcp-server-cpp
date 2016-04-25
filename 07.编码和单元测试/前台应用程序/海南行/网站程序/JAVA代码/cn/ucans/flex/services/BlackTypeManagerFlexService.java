package cn.ucans.flex.services;

import java.io.File;
import java.util.List;
import java.util.Map;

import jxl.Workbook;


import chobits.db.DBBuilder;
import chobits.db.DBFactory;
import chobits.init.ChobitsConfig;
import chobits.log.Logger;
import cn.ucans.bean.T_BLACK_WHITE;
import cn.ucans.bean.T_BLACK_WHITE_HIS;
import cn.ucans.flex.FlexReponse;
import cn.ucans.flex.FlexRequest;
import cn.ucans.param.ParamHandler;

public class BlackTypeManagerFlexService extends FlexService{
	
	private Logger logger = new Logger(BlackTypeManagerFlexService.class.getName());
	String strFileData = "";
	String strStyle = "";
	String strFileName = "";
	public FlexReponse doTransaction(FlexRequest request){
		logger.debug("-----------------------------�ڰ��������� ����-------------------------------");
		FlexReponse reponse = null;
		
		try{
			super.checkRole(request);
			int ope_id = request.ope_id;	
			switch(ope_id){
				case 2000:
					reponse = this.queryBlackTypeInfo(request);
					break;
				case 2001:
					reponse = this.queryHotelName(request);
					break;
				case 2002:
					reponse = this.get_T_Black_Type(request);
					break;
				case 2003:
					reponse = this.insert_T_Black_Type(request);
					break;
				case 2004:
					reponse = this.del_T_Black_Type(request);
					break;
				case 2005:
					reponse = this.CreateExcelFile(strFileData,strStyle,request);
					break;
				case 2006:
					reponse = this.DeleteExcelFile(strFileName, request);
					break;
				case 2007:
					reponse = this.queryWhiteTypeInfo(request);
					break;
				case 2008:
					reponse = this.addWhiteTypeInfo(request);
					break;
				case 2009:
					reponse = this.del_T_Write_Type(request);
					break;
				case 2010:
					reponse = this.queryBlackTypeHisInfo(request);
				break;
				case 2011:
					reponse = this.queryWhiteTypeHisInfo(request);
					break;
				case 2012:
					reponse = this.queryBlackTypeReport(request);
					break;
				default:
					break;
			}
			logger.debug("-----------------------------�ڰ��������� �ɹ����-------------------------------");
		}catch(Exception e){
			logger.error(e.getMessage());
			reponse = new FlexReponse(request,-1,e.getMessage());	
			logger.debug("-----------------------------�ڰ��������� ִ��ʧ��-------------------------------");
		}
		
		return reponse;
	}
	private FlexReponse queryBlackTypeInfo(FlexRequest request) throws Exception{
		
		T_BLACK_WHITE black = (T_BLACK_WHITE)request.getBean("black");	
		String sql = "select Org_ID,Phone_Number,Black_White_Type,to_char(Black_Type) Black_Type,Join_User_ID,to_char(Join_Time,'yyyy-mm-dd hh24:mi:ss') Join_Time,Notes from T_Black_White where Black_White_Type ='1' ";
		if(black.PHONE_NUMBER != "" && black.PHONE_NUMBER.length()>0){
			sql += " and PHONE_NUMBER='"+black.PHONE_NUMBER+"'";
		}
		if(black.ORG_ID != "" && black.ORG_ID.length()>0){
			sql += " and Org_ID='"+black.ORG_ID+"'";
		}
		sql +="order by Org_ID,Phone_Number";
		DBBuilder builder = new DBBuilder();
		List<Map<String,Object>> list  = builder.queryList(sql);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("��ȡ��������Ϣʱ����ԭ��"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list", list);
		return reponse;
	}
	private FlexReponse queryHotelName(FlexRequest request) throws Exception{
		ParamHandler builder = new ParamHandler();
		List<Map<String,Object>> list  = builder.get_T_Org_Info("");
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list", list);
		return reponse;
	}
	private FlexReponse get_T_Black_Type(FlexRequest request) throws Exception{
		ParamHandler builder = new ParamHandler();
		List<Map<String,Object>> list  = builder.get_T_Black_Type("");
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list", list);
		return reponse;
	}
	private FlexReponse insert_T_Black_Type(FlexRequest request) throws Exception{
		FlexReponse reponse = null;
		int count = 0;
		int countBlack = 0;
		T_BLACK_WHITE black = (T_BLACK_WHITE)request.getBean("black");	
		String sql = "insert into T_Black_White(Black_White_Type,Org_ID,Phone_Number,Black_Type,Join_User_ID,Join_Time,Notes) ";
		sql += " values('1','"+black.ORG_ID+"','"+black.PHONE_NUMBER+"',to_char("+black.BLACK_TYPE+"),'"+black.JOIN_USER_ID+"',sysdate,'"+black.NOTES+"')";
		DBFactory builder = new DBFactory();
		builder.openTransaction();
//		�жϺڰ������汾�з��иþƵ����Ϣ�����û�У������һ�����������ʱ��
		count = builder.getCount("T_Black_White_Version", " Black_White_Type='1' and Org_ID='"+black.ORG_ID+"'");
		if(count == 0){
			builder.excute("insert into T_Black_White_Version(Black_White_Type,Org_ID,Update_Time) values('1','"+black.ORG_ID+"',sysdate)");
			if(builder.SQL_Error_Code != 0){
				throw new Exception(" ���Ӻ������汾ʱ���� ������룺"+builder.SQL_Error_Code+" ������Ϣ��"+builder.SQL_Error_Message);
			}
		}else{
			builder.excute("update T_Black_White_Version set Update_Time=sysdate where Org_ID='"+black.ORG_ID+"' and Black_White_Type='1'");
			if(builder.SQL_Error_Code != 0){
				throw new Exception(" ���º������汾ʱ���� ������룺"+builder.SQL_Error_Code+" ������Ϣ"+builder.SQL_Error_Message);
			}
		}
//		�жϸþƵ�ĺ��������������Ϊ30����������Ӳ���ʾ����Ա�����������
		countBlack = builder.getCount("T_Black_White", " Black_White_Type='1' and Org_ID='"+black.ORG_ID+"'");
		if(countBlack >= 30){
			builder.interruptTransaction();
			throw new Exception("�Ƶ�ĺ������������ܳ���30����");
		}
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("�������:"+builder.SQL_Error_Code+" ������Ϣ:"+builder.SQL_Error_Message);
		}
//		ִ�����Ӻ���������
		builder.excute(sql);
		if(builder.SQL_Error_Code == 0){	
			reponse = new FlexReponse(request,0,"���Ӻ������ɹ���");
		}
		if(builder.SQL_Error_Code == 1){			
			throw new Exception("�þƵ��Ѿ����ڴ����͵ĺ��������룬��ȷ�ϣ�");
		}
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("�������:"+builder.SQL_Error_Code+" ������Ϣ:"+builder.SQL_Error_Message);
		}
		builder.closeTransaction();
		
		
		return reponse;
	}
	private FlexReponse del_T_Black_Type(FlexRequest request) throws Exception{
		T_BLACK_WHITE black = (T_BLACK_WHITE)request.getBean("delblack");
		String sql = "delete T_Black_White where Phone_Number='"+black.PHONE_NUMBER+"' and Black_White_Type='1' and Org_ID='"+black.ORG_ID+"'";
		DBFactory factory = new DBFactory();
		factory.openTransaction();
//		�����������ʷ����
		factory.excute("Insert Into T_Black_White_His(Black_White_Type,Org_ID,Phone_Number,Black_Type,Join_User_ID,Join_Time,Delete_User_ID,Delete_Time,Notes) "+
				" values('1','"+black.ORG_ID+"','"+black.PHONE_NUMBER+"',"+black.BLACK_TYPE+",'"+black.JOIN_USER_ID+"',"+
				"to_date('"+black.JOIN_TIME+"','yyyy-mm-dd hh24:mi:ss'),'"+request.user_id+"',sysdate,'"+black.NOTES+"')");
		if(factory.SQL_Error_Code != 0){			
			throw new Exception("������д����ʷ��ʱ�����������:"+factory.SQL_Error_Code+" ������Ϣ:"+factory.SQL_Error_Message);
		}
//		���º������汾�ŵ�ʱ��
		factory.excute("update T_Black_White_Version set Update_Time=sysdate where Org_ID='"+black.ORG_ID+"' and Black_White_Type='1'");
		if(factory.SQL_Error_Code != 0){			
			throw new Exception("���º������汾ʱ�����������:"+factory.SQL_Error_Code+" ������Ϣ:"+factory.SQL_Error_Message);
		}
//		ִ��ɾ������
		factory.excute(sql);
		if(factory.SQL_Error_Code != 0){			
			throw new Exception("�������:"+factory.SQL_Error_Code+" ������Ϣ:"+factory.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,"ɾ���ɹ���");
		factory.closeTransaction();
		if(factory.SQL_Error_Code != 0){			
			throw new Exception("�������:"+factory.SQL_Error_Code+" ������Ϣ:"+factory.SQL_Error_Message);
		}
		return reponse;
	}
	
	public FlexReponse CreateExcelFile(String strFileData,String strStyle,FlexRequest request) throws Exception 
	{
		
		File	oFile = null;
		String	strLine;
		String	strCol;
		String	strData = strFileData;
		int		nLenPlace = 0;
		int		nColPlace;

		try 
		{
			File	oDir = new File(ChobitsConfig.TempFileForderPath);
//			File	oDir = new File("/Tomcat/webapps/CCSS/XLSTmepFile");
//			File	oDir = new File("F:/MyEclipseWorkSpace/Souce/Happy_Space/Happy/WebRoot/TempFile");
			oFile = File.createTempFile("Excel",".xls",oDir);
			jxl.write.WritableWorkbook wwb = Workbook.createWorkbook(oFile);
			jxl.write.WritableSheet ws = wwb.createSheet("����������", 0);
			for(int nLine = 0;nLenPlace != -1;nLine ++)
			{
				nLenPlace  = strData.indexOf("&&");
				if(nLenPlace == -1)
				{
					strLine = strData;
				}
				else
				{
					strLine = strData.substring(0,nLenPlace);
					strData = strData.substring(nLenPlace + 2);
				}
				nColPlace = 0;
				for(int nCol = 0;nColPlace != -1;nCol ++)
				{
					nColPlace = strLine.indexOf("||");
					if(nColPlace == -1)
					{
						strCol = strLine;
					}
					else
					{
						strCol = strLine.substring(0,nColPlace);
						strLine = strLine.substring(nColPlace + 2);
					}
					jxl.write.Label labelC = new jxl.write.Label(nCol,nLine, strCol);
					ws.addCell(labelC);	
				}
			}
			nLenPlace = 0;
			for(;nLenPlace != -1;)
			{
				nLenPlace  = strStyle.indexOf("&");
				if(nLenPlace == -1)
				{
					strLine = strData;
				}
				else
				{
					strLine = strData.substring(0,nLenPlace);
					strData = strData.substring(nLenPlace + 2);
				}
			}
			wwb.write();
			wwb.close();
			String fileName = oFile.getName();
			FlexReponse reponse = new FlexReponse(request,0,"");
			reponse.org_id = fileName;
			return reponse;
		}
		catch (Exception e)
		{
			String error = "����xls�ļ��������� -- " + e.toString();
			 throw new Exception(error);
		}
	}
	
	public FlexReponse DeleteExcelFile(String strFileName,FlexRequest request) throws Exception 
	{
		String	strLine;
		try 
		{		
			java.io.File DelFile=new java.io.File(ChobitsConfig.TempFileForderPath+"/" + strFileName);
//			java.io.File DelFile=new java.io.File("F:/MyEclipseWorkSpace/Souce/Happy_Space/Happy/WebRoot/TempFile/" + strFileName);
			DelFile.delete();
			FlexReponse reponse = new FlexReponse(request,0,"ɾ��xls�ɹ���");
			return reponse;
		}
		catch (Exception e)
		{
			String error = "ɾ��xls�ļ��������� -- "+ e.toString();
			throw new Exception(error);
		}
	}
	private FlexReponse queryWhiteTypeInfo(FlexRequest request) throws Exception{
		
		String sql = "select Phone_Number,Join_User_ID,to_char(Join_Time,'yyyy-mm-dd hh24:mi:ss') Join_Time,Notes from T_Black_White where Black_White_Type ='0' ";
		sql +="order by Phone_Number";
		DBBuilder builder = new DBBuilder();
		List<Map<String,Object>> list  = builder.queryList(sql);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("��ȡ��������Ϣʱ����ԭ��"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list", list);
		return reponse;
	}
	private FlexReponse addWhiteTypeInfo(FlexRequest request) throws Exception{
		T_BLACK_WHITE white = (T_BLACK_WHITE)request.getBean("white");	
		String sql = "insert into T_Black_White(Black_White_Type,Phone_Number,Join_User_ID,Join_Time,Notes) values('0','"+white.PHONE_NUMBER+"','"+white.JOIN_USER_ID+"',sysdate,'"+white.NOTES+"')";
		DBFactory factroy = new DBFactory();
		int count = 0;
		int countBlack = 0;
		factroy.openTransaction();
//		�жϺڰ������汾�з��а汾��Ϣ�����û�У������һ�����������ʱ��
		count = factroy.getCount("T_Black_White_Version", " Black_White_Type='0'");
		if(count == 0){
			factroy.excute("insert into T_Black_White_Version(Black_White_Type,Update_Time) values('0',sysdate)");
			if(factroy.SQL_Error_Code != 0){
				throw new Exception(" ���Ӱ������汾ʱ���� ������룺"+factroy.SQL_Error_Code+" ������Ϣ��"+factroy.SQL_Error_Message);
			}
		}else{
			factroy.excute("update T_Black_White_Version set Update_Time=sysdate where Black_White_Type='0'");
			if(factroy.SQL_Error_Code != 0){
				throw new Exception(" ���°������汾ʱ���� ������룺"+factroy.SQL_Error_Code+" ������Ϣ"+factroy.SQL_Error_Message);
			}
		}
//		�жϰ��������������Ϊ10����������Ӳ���ʾ����Ա�����������
		countBlack = factroy.getCount("T_Black_White", " Black_White_Type='0'");
		if(countBlack >= 10){
			factroy.interruptTransaction();
			throw new Exception("�������������ܳ���10����");
		}
		if(factroy.SQL_Error_Code != 0){			
			throw new Exception("�������:"+factroy.SQL_Error_Code+" ������Ϣ:"+factroy.SQL_Error_Message);
		}
		
		factroy.excute(sql);
		if(factroy.SQL_Error_Code == 1){			
			throw new Exception("������İ������Ѵ��ڣ�");
		}
		if(factroy.SQL_Error_Code != 0){			
			throw new Exception("���Ӱ�������Ϣʱ����ԭ��"+factroy.SQL_Error_Code+" "+factroy.SQL_Error_Message);
		}
		factroy.closeTransaction();
		FlexReponse reponse = new FlexReponse(request,0,"��Ӱ������ɹ���");
		return reponse;
	}
	private FlexReponse del_T_Write_Type(FlexRequest request) throws Exception{
		T_BLACK_WHITE white = (T_BLACK_WHITE)request.getBean("delwhite");
		String sql = "delete T_Black_White where Phone_Number='"+white.PHONE_NUMBER+"' and Black_White_Type='0'";
		DBFactory factory = new DBFactory();
		factory.openTransaction();
//		�����������ʷ����
		factory.excute("Insert Into T_Black_White_His(Black_White_Type,Phone_Number,Join_User_ID,Join_Time,Delete_User_ID,Delete_Time,Notes) "+
				" values('0','"+white.PHONE_NUMBER+"','"+white.JOIN_USER_ID+"',"+
				"to_date('"+white.JOIN_TIME+"','yyyy-mm-dd hh24:mi:ss'),'"+request.user_id+"',sysdate,'"+white.NOTES+"')");
		if(factory.SQL_Error_Code != 0){			
			throw new Exception("������д����ʷ��ʱ�����������:"+factory.SQL_Error_Code+" ������Ϣ:"+factory.SQL_Error_Message);
		}
//		���°������汾�ŵ�ʱ��
		factory.excute("update T_Black_White_Version set Update_Time=sysdate where Black_White_Type='0'");
		if(factory.SQL_Error_Code != 0){			
			throw new Exception("���°������汾ʱ�����������:"+factory.SQL_Error_Code+" ������Ϣ:"+factory.SQL_Error_Message);
		}
//		ִ��ɾ������
		factory.excute(sql);
		if(factory.SQL_Error_Code != 0){			
			throw new Exception("�������:"+factory.SQL_Error_Code+" ������Ϣ:"+factory.SQL_Error_Message);
		}
		factory.closeTransaction();
		if(factory.SQL_Error_Code != 0){			
			throw new Exception("�������:"+factory.SQL_Error_Code+" ������Ϣ:"+factory.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,"ɾ���ɹ���");
		return reponse;
	}
	private FlexReponse queryBlackTypeHisInfo(FlexRequest request) throws Exception{
		T_BLACK_WHITE_HIS blackHis = (T_BLACK_WHITE_HIS)request.getBean("blackHis");
		String sql = "Select a.Org_ID Org_ID,b.Org_Name Org_Name,a.Phone_Number Phone_Number,c.Black_Name Black_Name,a.Join_User_ID Join_User_ID,a.Join_Time Join_Time,a.Delete_User_ID Delete_User_ID,a.Delete_Time Delete_Time,a.Notes Notes From T_Black_White_His a,T_Org_Info b, T_Black_Type c Where a.Org_ID=b.Org_ID and a.Black_White_Type ='1' and a.Black_Type=C.Black_Type ";
		if(blackHis.ORG_ID !="" && blackHis.ORG_ID.length()>0){
			sql +=" and a.Org_ID='"+blackHis.ORG_ID+"'";
		}
		if(blackHis.PHONE_NUMBER !="" && blackHis.PHONE_NUMBER.length()>0){
			sql +=" and Phone_Number='"+blackHis.PHONE_NUMBER+"'";
		}
		if(blackHis.JOIN_TIME != "" && blackHis.JOIN_TIME.length()>0 && blackHis.JOIN_USER_ID !="" && blackHis.JOIN_USER_ID.length()>0 ){
			sql +=" and to_char(a.Join_Time,'yyyy-mm-dd') >='"+blackHis.JOIN_TIME+"' and to_char(a.Join_Time,'yyyy-mm-dd') <='"+blackHis.JOIN_USER_ID+"'";
		}
		if(blackHis.DELETE_TIME != "" && blackHis.DELETE_TIME.length()>0 && blackHis.DELETE_USER_ID !="" && blackHis.DELETE_USER_ID.length()>0 ){
			sql +=" and to_char(a.Delete_Time,'yyyy-mm-dd') >='"+blackHis.DELETE_TIME+"' and to_char(a.Delete_Time,'yyyy-mm-dd') <='"+blackHis.DELETE_USER_ID+"'";
		}
		DBBuilder builder = new DBBuilder();
		List<Map<String,Object>> list  = builder.queryList(sql);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("��ȡ��������ʷ��Ϣʱ����ԭ��"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list", list);
		return reponse;
	}
	private FlexReponse queryWhiteTypeHisInfo(FlexRequest request) throws Exception{
		T_BLACK_WHITE_HIS whiteHis = (T_BLACK_WHITE_HIS)request.getBean("whiteHis");
		String sql = "Select Phone_Number,Join_User_ID,Join_Time,Delete_User_ID,Delete_Time,Notes From T_Black_White_His Where Black_White_Type ='0'";
		if(whiteHis.PHONE_NUMBER !="" && whiteHis.PHONE_NUMBER.length()>0){
			sql +=" and Phone_Number='"+whiteHis.PHONE_NUMBER+"'";
		}
		if(whiteHis.JOIN_TIME != "" && whiteHis.JOIN_TIME.length()>0 && whiteHis.JOIN_USER_ID !="" && whiteHis.JOIN_USER_ID.length()>0 ){
			sql +=" and to_char(Join_Time,'yyyy-mm-dd') >='"+whiteHis.JOIN_TIME+"' and to_char(Join_Time,'yyyy-mm-dd') <='"+whiteHis.JOIN_USER_ID+"'";
		}
		if(whiteHis.DELETE_TIME != "" && whiteHis.DELETE_TIME.length()>0 && whiteHis.DELETE_USER_ID !="" && whiteHis.DELETE_USER_ID.length()>0 ){
			sql +=" and to_char(Delete_Time,'yyyy-mm-dd') >='"+whiteHis.DELETE_TIME+"' and to_char(Delete_Time,'yyyy-mm-dd') <='"+whiteHis.DELETE_USER_ID+"'";
		}
		DBBuilder builder = new DBBuilder();
		List<Map<String,Object>> list  = builder.queryList(sql);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("��ȡ��������ʷ��Ϣʱ����ԭ��"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list", list);
		return reponse;
	}
	private FlexReponse queryBlackTypeReport(FlexRequest request) throws Exception{
		T_BLACK_WHITE_HIS whiteHis = (T_BLACK_WHITE_HIS)request.getBean("whiteHis");
		String sql = "Select rownum,Phone_Number,Counts From (Select Phone_Number,count(*) Counts From T_Black_White Where Black_White_Type='1' Group by Phone_Number Order by Counts desc) Where rownum<=10";
		DBBuilder builder = new DBBuilder();
		List<Map<String,Object>> list  = builder.queryList(sql);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("��ȡ������Top10��Ϣʱ����ԭ��"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list", list);
		return reponse;
	}
	

}
