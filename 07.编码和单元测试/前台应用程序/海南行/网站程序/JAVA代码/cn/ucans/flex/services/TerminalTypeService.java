package cn.ucans.flex.services;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;


import chobits.db.DBBuilder;
import chobits.log.Logger;
import cn.ucans.bean.T_TERMINAL_TYPE;
import cn.ucans.flex.FlexReponse;
import cn.ucans.flex.FlexRequest;
import cn.ucans.param.ParamHandler;

public class TerminalTypeService extends FlexService{
	
	private Logger logger = new Logger(TerminalTypeService.class.getName());
	
	public FlexReponse doTransaction(FlexRequest request){
		logger.debug("-----------------------------�ն����͹������� ����-------------------------------");
		FlexReponse reponse = null;
		
		try{
			super.checkRole(request);
			int ope_id = request.ope_id;	
			switch(ope_id){
				case 9000:
					reponse = this.initTerminalType(request);
					break;
				case 9001:
					reponse = this.addTerminalType(request);
					break;
				case 9002:
					reponse = this.queryTerminalType(request);
					break;
				case 9003:
					reponse = this.updateTerminalType(request);
					break;
				case 9004:
					reponse = this.deleteTerminalType(request);
					break;
				default:
					break;
			}
			logger.debug("-----------------------------�ն����͹������� �ɹ����-------------------------------");
		}catch(Exception e){
			logger.error(e.getMessage());
			reponse = new FlexReponse(request,-1,e.getMessage());	
			logger.debug("-----------------------------�ն����͹������� ִ��ʧ��-------------------------------");
		}
		
		return reponse;
	}
	
	private FlexReponse initTerminalType(FlexRequest request) throws Exception{
		
		String select_terminal_typeInfo="select T2.ORG_NAME ORG_NAME,T1.ORG_ID ORG_ID,T1.TERMINAL_TYPE TERMINAL_TYPE,T1.TERMINAL_NAME TERMINAL_NAME,T1.BASE_CONFIG BASE_CONFIG,T1.PERFORMANCE PERFORMANCE,T1.NOTES NOTES from T_TERMINAL_TYPE T1,T_ORG_INFO T2 where T1.ORG_ID=T2.ORG_ID Order by T2.ORG_NAME,T1.TERMINAL_TYPE";
		DBBuilder builder = new DBBuilder();
		List<Map<String,Object>> list = builder.queryList(select_terminal_typeInfo);
		
		List<Map<String,Object>> orgNameList = new ParamHandler().getT_OrgID__OrgName("1");
		
		List<Map<String,Object>> queryOrgNameList = new LinkedList<Map<String,Object>>();
		Map<String,Object> queryAllOrgListMap=new HashMap<String,Object>();
		queryAllOrgListMap.put("label", "ȫ��");
		queryAllOrgListMap.put("data", "");
		queryOrgNameList.add(queryAllOrgListMap);
		for(int i=0;i<orgNameList.size();i++){
			queryOrgNameList.add(orgNameList.get(i));
		}
		
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("��ѯ�ն�������Ϣ����ȡ�ն���Ϣ����ԭ��"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,null);	
		reponse.set("list", list);
		reponse.set("orgNameList", orgNameList);
		reponse.set("queryOrgNameList", queryOrgNameList);
		return reponse;
	}
	
	private FlexReponse queryTerminalType(FlexRequest request) throws Exception{
		T_TERMINAL_TYPE terminalType = (T_TERMINAL_TYPE) request.getBean("terminalType");
		String tempOrgId = terminalType.ORG_ID;
		String tempTerminalType = terminalType.TERMINAL_TYPE;
		String tempTerminalName = terminalType.TERMINAL_NAME;
		
		String select_terminal_typeInfo=null;
		if("".equals(tempOrgId)){
			select_terminal_typeInfo="select T2.ORG_NAME ORG_NAME,T1.ORG_ID ORG_ID,T1.TERMINAL_TYPE TERMINAL_TYPE,T1.TERMINAL_NAME TERMINAL_NAME,T1.BASE_CONFIG BASE_CONFIG,T1.PERFORMANCE PERFORMANCE,T1.NOTES NOTES from T_TERMINAL_TYPE T1,T_ORG_INFO T2 where T1.ORG_ID=T2.ORG_ID";
		}else{
			select_terminal_typeInfo="select T2.ORG_NAME ORG_NAME,T1.ORG_ID ORG_ID,T1.TERMINAL_TYPE TERMINAL_TYPE,T1.TERMINAL_NAME TERMINAL_NAME,T1.BASE_CONFIG BASE_CONFIG,T1.PERFORMANCE PERFORMANCE,T1.NOTES NOTES from T_TERMINAL_TYPE T1,T_ORG_INFO T2 where T1.ORG_ID=T2.ORG_ID and T1.ORG_ID='"+terminalType.ORG_ID+"'";	
			
		}
		
		if(tempTerminalType == null|| tempTerminalType.equals("")){
		}else{
			select_terminal_typeInfo+=" and T1.TERMINAL_TYPE like '%"+tempTerminalType+"%'";
		}
		if(tempTerminalName ==null||tempTerminalName.equals("")){		
		}else{
			select_terminal_typeInfo+=" and T1.TERMINAL_NAME like '%"+tempTerminalName+"%'";
		}
		
		select_terminal_typeInfo+=" Order by T2.ORG_NAME,T1.TERMINAL_TYPE";
		
		
		DBBuilder builder = new DBBuilder();
		List<Map<String,Object>> list = builder.queryList(select_terminal_typeInfo);;
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("��ѯ�ն�������Ϣ����ȡ�ն���Ϣ����ԭ��"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list", list);
		return reponse;
	}
	
	private FlexReponse addTerminalType(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();
		T_TERMINAL_TYPE terminalType = (T_TERMINAL_TYPE) request.getBean("terminalType");
		int count = builder.getCount("T_TERMINAL_TYPE", "Org_ID='"+terminalType.ORG_ID+"' and Terminal_Type='"+terminalType.TERMINAL_TYPE+"'");
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("���ò���������ն����ͣ���ȡ�ն�������Ϣ����ԭ��"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}else{
			if(count>0){
				throw new Exception("���ò���������ն����ͣ��ñ�ţ�"+terminalType.TERMINAL_TYPE+"���Ѿ����ڣ�");
			}
		}
		
		String sql = "Insert Into T_Terminal_Type(Org_ID,Terminal_Type,Terminal_Name,Base_Config,Performance,Notes) Values('"
			+terminalType.ORG_ID+ "','"+terminalType.TERMINAL_TYPE+"','" +terminalType.TERMINAL_NAME+ "','"+terminalType.BASE_CONFIG+"','"+terminalType.PERFORMANCE+"','"+terminalType.NOTES+"')";
		builder.excute(sql);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("���ò���������ն����ͳ���ԭ��"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,"����ն����ͣ�"+terminalType.TERMINAL_TYPE+"��"+terminalType.TERMINAL_NAME+"����Ϣ�ɹ���");
		reponse.set("terminalType", terminalType);
		return reponse;
	}
	
	private FlexReponse updateTerminalType(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();
		
		T_TERMINAL_TYPE terminalType = (T_TERMINAL_TYPE) request.getBean("terminalType");
		T_TERMINAL_TYPE oldTerminalType = (T_TERMINAL_TYPE) request.getBean("oldTerminalType");
		
		int count = builder.getCount("T_TERMINAL_TYPE", " Org_ID='"+oldTerminalType.ORG_ID+"' and Terminal_Type='"+oldTerminalType.TERMINAL_TYPE+"'  and Terminal_Name='"+oldTerminalType.TERMINAL_NAME+"' and Base_Config='"+oldTerminalType.BASE_CONFIG+"' and Performance='"+oldTerminalType.PERFORMANCE+"'");
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("���ò������޸��ն����ͣ���ȡ�ն�������Ϣ����ԭ��"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}else{
			if(count==0){
				throw new Exception("���ò������޸��ն����ͣ��ã�"+oldTerminalType.TERMINAL_TYPE+"���ն��ͺ��Ѿ������ڻ��ѱ��޸ģ���ȷ�Ϻ����޸ģ�");
			}
		}
		
		String update_sql = "update T_Terminal_Type set ";
		update_sql += " Terminal_Type ='"+terminalType.TERMINAL_TYPE+"',";
		update_sql += " Terminal_Name ='"+terminalType.TERMINAL_NAME+"',";
		update_sql += " Base_Config ='"+terminalType.BASE_CONFIG+"',";
		update_sql += " Performance ='"+terminalType.PERFORMANCE+"',";
		update_sql += " Notes ='"+terminalType.NOTES+"'";
		update_sql += " where Terminal_Type='"+ oldTerminalType.TERMINAL_TYPE +"' and Org_ID='"+oldTerminalType.ORG_ID+"'";
		builder.excute(update_sql);
		
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("���ò������޸��ն����ͳ���ԭ��"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		
		FlexReponse reponse = new FlexReponse(request,0,"�޸��ն�������Ϣ�ɹ���");
		
		return reponse;
	}
	
	private FlexReponse deleteTerminalType(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();
		T_TERMINAL_TYPE terminalType = (T_TERMINAL_TYPE) request.getBean("terminalType");
		
		int count = builder.getCount("T_TERMINAL_TYPE", "Org_ID='"+terminalType.ORG_ID+"'and Terminal_Type='"+terminalType.TERMINAL_TYPE+"'");
		
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("���ò�����ɾ���ն����ͣ���ȡ�ն�������Ϣ����ԭ��"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}else{
			if(count==0){
				throw new Exception("���ò�����ɾ���ն����ͣ��ã�"+terminalType.TERMINAL_TYPE+"���ն��ͺ��Ѿ������ڻ��Ѿ����޸ģ���ȷ�Ϻ���ɾ����");
			}
		}
		
		
		String sql = "delete from T_TERMINAL_TYPE where Org_ID = '"+terminalType.ORG_ID+"' and Terminal_Type='"+ terminalType.TERMINAL_TYPE+"'";
		builder.excute(sql);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("���ò�����ɾ���ն�����ʧ�ܣ�ԭ��"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,"ɾ���ն����ͣ�"+terminalType.ORG_ID+"��"+terminalType.TERMINAL_TYPE+"����Ϣ�ɹ���");
		return reponse;
	}
}
