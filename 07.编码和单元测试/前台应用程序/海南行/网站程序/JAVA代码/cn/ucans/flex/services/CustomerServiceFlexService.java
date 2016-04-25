package cn.ucans.flex.services;

import java.util.List;
import java.util.Map;


import chobits.db.DBBuilder;
import chobits.log.Logger;
import cn.ucans.bean.T_ORG_INFO;
import cn.ucans.flex.FlexReponse;
import cn.ucans.flex.FlexRequest;

public class CustomerServiceFlexService extends FlexService{
	
	private Logger logger = new Logger(CustomerServiceFlexService.class.getName());
	
	public FlexReponse doTransaction(FlexRequest request){
		logger.debug("-----------------------------�ͻ��������� ����-------------------------------");
		FlexReponse reponse = null;
		
		try{
			super.checkRole(request);
			
			int ope_id = request.ope_id;	
			switch(ope_id){
				case 8001:
					reponse = this.addTerminalFactory(request);
					break;
				case 8002:
					reponse = this.updateTerminalFactory(request);
					break;
				case 8003:
					reponse = this.deleteTerminalFactory(request);
					break;
				case 8004:
					reponse = this.queryTerminalFactory(request);
					break;
				default:
					break;
			}
			logger.debug("-----------------------------�ͻ��������� �ɹ����-------------------------------");
		}catch(Exception e){
			logger.error(e.getMessage());
			reponse = new FlexReponse(request,-1,e.getMessage());	
			logger.debug("-----------------------------�ͻ��������� ִ��ʧ��-------------------------------");
		}		
		return reponse;
	}
	
	private FlexReponse addTerminalFactory(FlexRequest request) throws Exception{
		T_ORG_INFO org = (T_ORG_INFO) request.getBean("org");
		DBBuilder builder = new DBBuilder();
		int count = builder.getCount("T_ORG_INFO", "Org_ID='"+org.ORG_ID+"' and Org_Type='1'");
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("�ͻ���������ն˳�����Ϣ��У�鳧�̱�ţ�ԭ��"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}else{
			if(count>0){
				throw new Exception("�ͻ���������ն˳�����Ϣ���ñ�ţ�"+org.ORG_ID+"���Ѿ����ڣ�");
			}
		}
		String sql_insert_org = "insert into T_ORG_INFO (ORG_ID,ORG_NAME,ORG_TYPE,SUPERIOR_ORG_ID,TOP_LEVEL,ADDRESS,ZIP_CODE,FAX_NO,Website,STATUS,CREATE_USER_ID,CREATE_TIME,LAST_UPDATE_TIME,NOTES) values ";
		sql_insert_org += "('"+org.ORG_ID+"','"+org.ORG_NAME+"','1','HAPPY',1,'"+org.ADDRESS+"','"+org.ZIP_CODE+"','"+org.FAX_NO+"','"+org.WEBSITE+"','0','"+org.CREATE_USER_ID+"',sysdate,sysdate,'"+org.NOTES+"')";
		builder.excute(sql_insert_org);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("�ͻ���������ն˳�����Ϣ����ԭ��"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,"����ն˳��̣�"+org.ORG_NAME+"����Ϣ�ɹ���");
		return reponse;
	}
	
	private FlexReponse updateTerminalFactory(FlexRequest request) throws Exception{
		T_ORG_INFO org = (T_ORG_INFO) request.getBean("org");
		DBBuilder builder = new DBBuilder();
		int count = builder.getCount("T_ORG_INFO", "Org_ID='"+org.ORG_ID+"' and Status='0' and Org_Type='1'");
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("�ͻ����񣬸����ն˳�����Ϣ��У�鳧�̱�ţ�ԭ��"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}else{
			if(count==0){
				throw new Exception("�ͻ����񣬸����ն˳�����Ϣ���ñ�ţ�"+org.ORG_ID+"���Ѿ������ڣ�");
			}
		}
		
		String sql_update_org = "update T_ORG_INFO set ";
		sql_update_org += " Org_Name = '"+org.ORG_NAME+"',";//
		sql_update_org += " Address = '"+org.ADDRESS+"',";//
		sql_update_org += " Zip_Code = '"+org.ZIP_CODE+"',";//
		sql_update_org += " Status = '"+org.STATUS+"',";//
		sql_update_org += " Last_Update_Time = sysdate,";//
		sql_update_org += " Notes = '"+org.NOTES+"' ";//
		sql_update_org += " where ORG_ID = '"+org.ORG_ID+"'"+" and Status='0' and Org_Type='1'";
		builder.excute(sql_update_org);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("�ͻ����񣬸����ն˳�����Ϣ����ԭ��"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,"�����ն˳��̣�"+org.ORG_NAME+"����Ϣ�ɹ���");
		return reponse;
	}
	
	private FlexReponse deleteTerminalFactory(FlexRequest request) throws Exception{
		T_ORG_INFO org = (T_ORG_INFO) request.getBean("org");
		DBBuilder builder = new DBBuilder();
		
		String delete_sql = "update T_ORG_INFO set Status='1' where Org_ID='"+org.ORG_ID+"' and Status='0' and Org_Type='1'";
		builder.excute(delete_sql);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("�ͻ�����ɾ���ն˳�����Ϣ����ԭ��"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,"ɾ���ն˳��̣�"+org.ORG_NAME+"����Ϣ�ɹ���");
		return reponse;
	}
	
	private FlexReponse queryTerminalFactory(FlexRequest request) throws Exception{
		T_ORG_INFO org = (T_ORG_INFO) request.getBean("org");
		DBBuilder builder = new DBBuilder();
		
		String query_sql = "select ORG_ID,ORG_NAME,ORG_TYPE,SUPERIOR_ORG_ID,TOP_LEVEL,ADDRESS,ZIP_CODE,FAX_NO,Website,STATUS,CREATE_USER_ID,CREATE_TIME,LAST_UPDATE_TIME,NOTES from T_ORG_INFO ";
		query_sql += " where Org_Type='1' ";
		if(org.STATUS != null && org.STATUS.trim().length()>0){
			query_sql += "  and STATUS = '"+org.STATUS+"'";
		}
		if(org.ORG_NAME != null && org.ORG_NAME.trim().length()>0){
			query_sql += "  and ORG_NAME like '%"+org.ORG_NAME+"%'";
		}	
		query_sql += " order by Create_Time";
		List<Map<String,Object>> list = builder.queryList(query_sql);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("�ͻ���������ն˳�����Ϣ����ԭ��"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list", list);
		return reponse;
	}
}
