package cn.ucans.flex.services;

import java.io.File;
import java.util.List;
import java.util.Map;

import jxl.Workbook;


import chobits.db.DBBuilder;
import chobits.init.ChobitsConfig;
import chobits.log.Logger;
import cn.ucans.flex.FlexReponse;
import cn.ucans.flex.FlexRequest;
import cn.ucans.param.ParamHandler;

public class StatementRetalHistoryForPlatformService extends FlexService{
	
	private Logger logger = new Logger(TerminalTypeService.class.getName());
	
	public FlexReponse doTransaction(FlexRequest request){
		logger.debug("-----------------------------�ն�������ʷ��ƽ̨���������� ����-------------------------------");
		FlexReponse reponse = null;
		
		try{
			super.checkRole(request);
			int ope_id = request.ope_id;	
			switch(ope_id){
				case 9110:
					reponse = this.initStatement(request);
					break;
				case 9111:
					reponse = this.QueryStatement(request);
					break;
				default:
					break;
			}
			logger.debug("-----------------------------�ն�������ʷ��ƽ̨���������� �ɹ����-------------------------------");
		}catch(Exception e){
			logger.error(e.getMessage());
			reponse = new FlexReponse(request,-1,e.getMessage());	
			logger.debug("-----------------------------�ն�������ʷ��ƽ̨���������� ִ��ʧ��-------------------------------");
		}
		
		return reponse;
	}
	
	private FlexReponse initStatement(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();
		
		String startTime=(String)request.getBean("startTime");
		String endTime=(String)request.getBean("endTime");
		
		String sql="Select c.Org_Name ORG_AGENT_NAME,b.Org_Name ORG_HOTEL_NAME,a.Room_No,a.Msisdn,a.Start_Time,a.End_Time,round(a.End_Time-a.Start_Time,2) TIME_LENGTH,a.Cash/100 CASH,a.Real_Pay_Cash/100 REAL_PAY_CASH,a.Status" +
				" From V_Service_Cash a,T_Org_Info b,T_Org_Info c " +
				"Where a.Hotel_ID=b.Org_ID and a.Broker_ID=c.Org_ID";
		
		
		if(!"".equals(startTime)){
			sql+=" and to_char(a.start_time,'yyyy-MM-dd') >='"+startTime+"'";
		}
		if(!"".equals(endTime)){
			endTime+=" 23:59:59";
			sql+=" and to_char(a.start_time,'yyyy-MM-dd HH24:MI:SS') <='"+endTime+"'";
		}
		
		List<Map<String,Object>> list = builder.queryList(sql);
		
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("��ѯ�ն�������ʷ��ƽ̨����Ϣ����ȡ�ն�������ʷ��ƽ̨������ԭ��"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		List<Map<String,Object>> queryHotelNameList = new ParamHandler().getT_OrgID__OrgNameAll("3");
		List<Map<String,Object>> queryAgentNameList = new ParamHandler().getT_OrgID__OrgNameAll("2");
		
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("��ѯ�ն�������ʷ��ƽ̨����Ϣ����ȡ�ն�������ʷ��ƽ̨������ԭ��"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,null);	
		reponse.set("list", list);
		reponse.set("queryHotelNameList", queryHotelNameList);
		reponse.set("queryAgentNameList", queryAgentNameList);
		return reponse;
	}
	
	private FlexReponse QueryStatement(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();
		String hotel=(String)request.getBean("hotel");
		String agent=(String)request.getBean("agent");
		String startTime=(String)request.getBean("startTime");
		String endTime=(String)request.getBean("endTime");
		String roomNo=(String)request.getBean("roomNo");
		String telNo=(String)request.getBean("telNo");
		
		String sql="Select c.Org_Name ORG_AGENT_NAME,b.Org_Name ORG_HOTEL_NAME,a.Room_No,a.Msisdn,a.Start_Time,a.End_Time,round(a.End_Time-a.Start_Time,2) TIME_LENGTH,a.Cash/100 CASH,a.Real_Pay_Cash/100 REAL_PAY_CASH,a.Status" +
		" From V_Service_Cash a,T_Org_Info b,T_Org_Info c " +
		"Where a.Hotel_ID=b.Org_ID and a.Broker_ID=c.Org_ID";
		
		if(!"".equals(hotel)){
			sql+=" and b.Org_Id='"+hotel+"'";
		}
		if(!"".equals(agent)){
			sql+=" and c.Org_Id='"+agent+"'";
		}
		if(!"".equals(startTime)){
			sql+=" and to_char(a.start_time,'yyyy-MM-dd') >='"+startTime+"'";
		}
		if(!"".equals(endTime)){
			endTime+=" 23:59:59";
			sql+=" and to_char(a.start_time,'yyyy-MM-dd HH24:MI:SS') <='"+endTime+"'";
		}
		
		if(!"".equals(roomNo)){
			sql+=" and a.Room_No='"+roomNo+"'";
		}
		
		if(!"".equals(telNo)){
			sql+=" and a.Msisdn='"+telNo+"'";
		}
		
		
		List<Map<String,Object>> list = builder.queryList(sql);
		
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("ѯ�ն�������ʷ��ƽ̨����Ϣ����ȡ�ն�������ʷ��ƽ̨������ԭ��"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,null);	
		reponse.set("list", list);
		return reponse;
	}
	
	/**
	 * ��������ΪExcel��
	 * @param strFileData
	 * @param strStyle
	 * @param request
	 * @return
	 * @throws Exception
	 */
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
//			File	oDir = new File("F:\\workspace\\source\\Happy_Spase\\Happy\\WebRoot\\TempFile");
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
			reponse.set("fileName", fileName);
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
		try 
		{		
			java.io.File DelFile=new java.io.File(ChobitsConfig.TempFileForderPath+"/" + strFileName);
			//java.io.File DelFile=new java.io.File("F:\\workspace\\source\\Happy_Spase\\Happy\\WebRoot\\TempFile\\" + strFileName);
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
}
