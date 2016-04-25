package cn.ucans.flex.services;

import java.io.File;
import java.util.List;
import java.util.Map;

import chobits.common.FileUtil;
import chobits.db.DBBuilder;
import chobits.db.DBFactory;
import chobits.init.ChobitsConfig;
import chobits.log.Logger;
import cn.ucans.adapter.OrgAdapter;
import cn.ucans.bean.T_ORG_DOCUMENT;
import cn.ucans.bean.T_ORG_INFO;
import cn.ucans.flex.FlexReponse;
import cn.ucans.flex.FlexRequest;
import cn.ucans.param.ParamHandler;

public class OrgFlexService extends FlexService{
	
	private Logger logger = new Logger(OrgFlexService.class.getName());
	
	public FlexReponse doTransaction(FlexRequest request){
		logger.debug("-----------------------------��֯��Ϣ�������� ����-------------------------------");
		FlexReponse reponse = null;
		
		try{			
			super.checkRole(request);
			
			int ope_id = request.ope_id;			
			switch(ope_id){
				case 1001:
					reponse = this.addOrg(request);
					break;
				case 1002:
					reponse = this.updateOrg(request);
					break;
				case 1003:
					reponse = this.deleteOrg(request);
					break;
				case 1004:
					reponse = this.queryOrg(request);
					break;
				case 1005:
					reponse = this.initOrgTreeNode(request);
					break;
				case 1006:
					reponse = this.queryCommonOrg(request);
					break;
				case 1010:
					reponse = this.initDocument(request);
					break;
				case 1011:
					reponse = this.addDocument(request);
					break;
				case 1012:
					reponse = this.updateDocument(request);
					break;
				case 1013:
					reponse = this.deleteDocument(request);
					break;
				case 1014:
					reponse = this.queryDocument(request);
					break;
				case 1015:
					reponse = this.viewDocument(request);
					break;
				default:
					break;
			}
			logger.debug("-----------------------------��֯��Ϣ�������� �ɹ����-------------------------------");
		}catch(Exception e){
			logger.error(e.getMessage());
			reponse = new FlexReponse(request,-1,e.getMessage());	
			logger.debug("-----------------------------��֯��Ϣ�������� ִ��ʧ��-------------------------------");
		}
		
		return reponse;
	}
	
	private FlexReponse addOrg(FlexRequest request) throws Exception{	
		
		String solution = this.getSolution(request.type);

		DBFactory factory = new DBFactory();
		factory.openTransaction();
		T_ORG_INFO org = (T_ORG_INFO) request.getBean("org");
		Map<String,Object> parm_map = (Map<String, Object>) request.getBean("parm");
		
		int count = factory.getCount("T_ORG_INFO", " ORG_ID='"+org.ORG_ID+"' "+solution);
		if(factory.SQL_Error_Code != 0){
			throw new Exception("�����֯��Ϣ��������֯��ų���"+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}else{
			if(count > 0){
				factory.interruptTransaction();
				throw new Exception("�����֯��Ϣ����ԭ����֯��ţ�"+org.ORG_ID+"���Ѿ����ڣ�");
			}
		}
		
		int count_name = factory.getCount("T_ORG_INFO", " ORG_Name='"+org.ORG_NAME+"' "+solution);
		if(factory.SQL_Error_Code != 0){
			throw new Exception("�����֯��Ϣ��������֯��ų���"+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}else{
			if(count_name > 0){
				factory.interruptTransaction();
				throw new Exception("�����֯��Ϣ����ԭ����֯���ƣ�"+org.ORG_ID+"���Ѿ����ڣ�");
			}
		}
		
		String sql_insert_org = null;
		if(org.ORG_TYPE.equals("3")){
			sql_insert_org = "insert into T_ORG_INFO (ORG_ID,ORG_NAME,ORG_TYPE,SUPERIOR_ORG_ID,TOP_LEVEL,HOTEL_GRADE,ADDRESS,ZIP_CODE,FAX_NO,CONTRACT_NO,CONTRACT_DATE,Website,ROOM_COUNT,AVERAGE_PRICE,SEND_SM_MAX_DAILY,STATUS,CREATE_USER_ID,CREATE_TIME,LAST_UPDATE_TIME,NOTES) values ";
			sql_insert_org += "('"+org.ORG_ID+"','"+org.ORG_NAME+"','"+org.ORG_TYPE+"','"+org.SUPERIOR_ORG_ID+"',"+org.TOP_LEVEL+","+org.HOTEL_GRADE+",'"+org.ADDRESS+"','"+org.ZIP_CODE+"','"+org.FAX_NO+"','"+org.CONTRACT_NO+"','"+org.CONTRACT_DATE.replaceAll("-", "")+"','"+org.WEBSITE+"',"+org.ROOM_COUNT+","+org.AVERAGE_PRICE+","+org.SEND_SM_MAX_DAILY+",'"+org.STATUS+"','"+org.CREATE_USER_ID+"',sysdate,sysdate,'"+org.NOTES+"')";
		}else{
			sql_insert_org = "insert into T_ORG_INFO (ORG_ID,ORG_NAME,ORG_TYPE,SUPERIOR_ORG_ID,TOP_LEVEL,ADDRESS,ZIP_CODE,FAX_NO,Website,STATUS,CREATE_USER_ID,CREATE_TIME,LAST_UPDATE_TIME,NOTES) values ";
			sql_insert_org += "('"+org.ORG_ID+"','"+org.ORG_NAME+"','"+org.ORG_TYPE+"','"+org.SUPERIOR_ORG_ID+"',"+org.TOP_LEVEL+",'"+org.ADDRESS+"','"+org.ZIP_CODE+"','"+org.FAX_NO+"','"+org.WEBSITE+"','"+org.STATUS+"','"+org.CREATE_USER_ID+"',sysdate,sysdate,'"+org.NOTES+"')";
		}	
		factory.excute(sql_insert_org);
		if(factory.SQL_Error_Code != 0){
			throw new Exception("�����֯��Ϣ��������"+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		
		if(org.ORG_TYPE.equals("2")||org.ORG_TYPE.equals("3")||org.ORG_TYPE.equals("4")){
			
			//'Reward_SM','Reward_Hire','Reward_FEE','Reward_IPFEE'
			String Reward_SM = (String) parm_map.get("Reward_SM");
			String Reward_HIRE = (String) parm_map.get("Reward_HIRE");
			String Reward_FEE = (String) parm_map.get("Reward_FEE");
			String Reward_IPFEE = (String) parm_map.get("Reward_IPFEE");
			
			String sql_p1 = "insert Into T_Org_Reward(Org_ID,Reward_Type,Reward_Percent,Create_User_ID,Create_Time,Last_Update_Time,Notes) values('"+org.ORG_ID+"',1,"+Reward_HIRE+",'"+request.user_id+"',sysdate,sysdate,'����ѷֳɱ���')";
			factory.excute(sql_p1);
			if(factory.SQL_Error_Code != 0){
				throw new Exception("�����֯��Ϣ����������ѷֳɱ�������ԭ��"+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
			}
			String sql_p2 = "insert Into T_Org_Reward(Org_ID,Reward_Type,Reward_Percent,Create_User_ID,Create_Time,Last_Update_Time,Notes) values('"+org.ORG_ID+"',2,"+Reward_SM+",'"+request.user_id+"',sysdate,sysdate,'���ѷֳɱ���')";
			factory.excute(sql_p2);
			if(factory.SQL_Error_Code != 0){
				throw new Exception("�����֯��Ϣ��������ѷֳɱ�������ԭ��"+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
			}
			String sql_p3 = "insert Into T_Org_Reward(Org_ID,Reward_Type,Reward_Percent,Create_User_ID,Create_Time,Last_Update_Time,Notes) values('"+org.ORG_ID+"',3,"+Reward_FEE+",'"+request.user_id+"',sysdate,sysdate,'ͨ���ѷֳɱ���')";
			factory.excute(sql_p3);
			if(factory.SQL_Error_Code != 0){
				throw new Exception("�����֯��Ϣ������ͨ���ѷֳɱ�������ԭ��"+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
			}
			String sql_p4 = "insert Into T_Org_Reward(Org_ID,Reward_Type,Reward_Percent,Create_User_ID,Create_Time,Last_Update_Time,Notes) values('"+org.ORG_ID+"',4,"+Reward_IPFEE+",'"+request.user_id+"',sysdate,sysdate,'IP���ѷֳɱ���')";
			factory.excute(sql_p4);
			if(factory.SQL_Error_Code != 0){
				throw new Exception("�����֯��Ϣ������IP���ѷֳɱ�������ԭ��"+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
			}
		}
		
		factory.closeTransaction();
		if(factory.SQL_Error_Code != 0){
			throw new Exception("�����֯��Ϣ���ύ��֯����������ԭ��"+factory.SQL_Error_Code+" "+factory.SQL_Error_Message);
		}
		
		
		OrgAdapter oa = new OrgAdapter();
		String xml = oa.getTreeXMLString(org.SUPERIOR_ORG_ID, Integer.parseInt(org.TOP_LEVEL)-1,solution);
		if(xml == null){
			throw new Exception("�����֯��Ϣ��������֯�ڵ�("+org.ORG_ID+","+org.TOP_LEVEL+")���ݳ�������֯("+org.ORG_ID+","+org.TOP_LEVEL+")�����ڣ�");
		}else{
			logger.debug("��ѯ��Ӻ����֯����"+xml);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,"�����֯("+org.ORG_ID+"��"+org.ORG_NAME+")�ɹ���");
		reponse.set("xml", xml);
		return reponse;
	}
	
	private FlexReponse updateOrg(FlexRequest request) throws Exception{
		
		String solution = this.getSolution(request.type);
		
		DBBuilder builder = new DBBuilder();
		T_ORG_INFO org = (T_ORG_INFO) request.getBean("org");
		
		int count = builder.getCount("T_ORG_INFO", " ORG_ID='"+org.ORG_ID+"' "+solution);
		if(builder.SQL_Error_Code != 0){
			throw new Exception("������֯��Ϣ��������֯��ų���"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}else{
			if(count==0){
				throw new Exception("������֯��Ϣ����ԭ����֯��ţ�"+org.ORG_ID+"���Ѿ������ڣ�");
			}
		}
		String org_name_old = (String) request.getBean("org_name_old");
		if(org_name_old.equals(org.ORG_NAME)){
			
		}else{
			int count_name = builder.getCount("T_ORG_INFO", " ORG_Name='"+org.ORG_NAME+"' "+solution);
			if(builder.SQL_Error_Code != 0){
				throw new Exception("������֯��Ϣ��������֯��ų���"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
			}else{
				if(count_name > 0){
					throw new Exception("������֯��Ϣ����ԭ����֯���ƣ�"+org.ORG_ID+"���Ѿ����ڣ�");
				}
			}
		}
		
		String sql_update_org = "update T_ORG_INFO set ";
		if(org.ORG_TYPE.equals("3")){
			sql_update_org += " Org_Name = '"+org.ORG_NAME+"',";//
			sql_update_org += " Org_Type = '"+org.ORG_TYPE+"',";//
			//sql_update_org += " Superior_Org_ID = '"+org.SUPERIOR_ORG_ID+"',";//
			sql_update_org += " Hotel_Grade = "+org.HOTEL_GRADE+",";//
			sql_update_org += " AVERAGE_PRICE = "+org.AVERAGE_PRICE+",";//
			sql_update_org += " Address = '"+org.ADDRESS+"',";//
			sql_update_org += " Zip_Code = '"+org.ZIP_CODE+"',";//
			sql_update_org += " WEBSITE = '"+org.WEBSITE+"',";//
			sql_update_org += " Contract_No = '"+org.CONTRACT_NO+"',";//
			sql_update_org += " Contract_Date = '"+org.CONTRACT_DATE+"',";//
			sql_update_org += " Room_Count = "+org.ROOM_COUNT+",";//
			sql_update_org += " Send_SM_Max_Daily = "+org.SEND_SM_MAX_DAILY+",";//
			sql_update_org += " Status = '"+org.STATUS+"',";//
			//sql_update_org += " Create_User_ID = '"+org.CREATE_USER_ID+"',";//
			sql_update_org += " Last_Update_Time = sysdate,";//
			sql_update_org += " Notes = '"+org.NOTES+"' ";//
		}else{
			sql_update_org += " Org_Name = '"+org.ORG_NAME+"',";//
			sql_update_org += " Org_Type = '"+org.ORG_TYPE+"',";//
			//sql_update_org += " Superior_Org_ID = '"+org.SUPERIOR_ORG_ID+"',";//
			//sql_update_org += " Hotel_Grade = "+org.HOTEL_GRADE+",";//
			sql_update_org += " Address = '"+org.ADDRESS+"',";//
			sql_update_org += " Zip_Code = '"+org.ZIP_CODE+"',";//
			sql_update_org += " WEBSITE = '"+org.WEBSITE+"',";//
			//sql_update_org += " Contract_No = '"+org.CONTRACT_NO+"',";//
			//sql_update_org += " Contract_Date = '"+org.CONTRACT_DATE+"',";//
			//sql_update_org += " Room_Count = "+org.ROOM_COUNT+",";//
			//sql_update_org += " Send_SM_Max_Daily = "+org.SEND_SM_MAX_DAILY+",";//
			sql_update_org += " Status = '"+org.STATUS+"',";//
			//sql_update_org += " Create_User_ID = '"+org.CREATE_USER_ID+"',";//
			sql_update_org += " Last_Update_Time = sysdate,";//
			sql_update_org += " Notes = '"+org.NOTES+"' ";//
		}	
		sql_update_org += " where ORG_ID = '"+org.ORG_ID+"'"+" and Status='0'";
		builder.excute(sql_update_org);			
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("������֯��Ϣ��������"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		String sql_select_org = "select ORG_ID,ORG_NAME,ORG_TYPE,SUPERIOR_ORG_ID,TOP_LEVEL,HOTEL_GRADE,ADDRESS,ZIP_CODE,FAX_NO,CONTRACT_NO,CONTRACT_DATE,Website,ROOM_COUNT,AVERAGE_PRICE,SEND_SM_MAX_DAILY,STATUS,CREATE_USER_ID,CREATE_TIME,LAST_UPDATE_TIME,NOTES from T_ORG_INFO ";
		sql_select_org += " where ORG_ID='"+org.ORG_ID+"'"+" "+solution;
		Map<String,Object> map_org_selected = builder.queryMap(sql_select_org);
		if(builder.SQL_Error_Code != 0){
			throw new Exception("������֯��Ϣ����ѯ�����ڵ㷢������ԭ��"+builder.SQL_Error_Code+","+builder.SQL_Error_Message);
		}
		
		OrgAdapter oa = new OrgAdapter();
		String xml = oa.getTreeXMLString(org.ORG_ID, Integer.parseInt(org.TOP_LEVEL),solution);
		if(xml == null){
			throw new Exception("������֯��Ϣ��������֯�ڵ�("+org.ORG_ID+","+org.TOP_LEVEL+")���ݳ�������֯("+org.ORG_ID+","+org.TOP_LEVEL+")�����ڣ�");
		}else{
			logger.debug("��ѯ���º����֯����"+xml);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,"������֯("+org.ORG_ID+"��"+org.ORG_NAME+")��Ϣ�ɹ���");
		reponse.set("map", map_org_selected);
		reponse.set("xml", xml);
		return reponse;
	}

	private FlexReponse deleteOrg(FlexRequest request) throws Exception{
		
		String solution = this.getSolution(request.type);
		
		DBBuilder builder = new DBBuilder();
		T_ORG_INFO org = (T_ORG_INFO) request.getBean("org");	
		
		int count_org = builder.getCount("T_ORG_INFO", "Superior_Org_ID='"+org.ORG_ID+"' and Top_Level="+(Integer.parseInt(org.TOP_LEVEL)+1)+solution);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("ɾ����֯��Ϣ����ѯ�丸�ڵ��µĺ��ӽڵ����ʱ��������"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}else{
			if(count_org > 0){
				throw new Exception("ɾ����֯��Ϣʱ����ԭ�򣺸���֯����"+count_org+"���¼���֯�����ȴ�����Щ��֯���ٽ��б�������");
			}
		}
		
//		int count_linkman = builder.getCount("T_ORG_LINKMAN", "Org_ID='"+org.ORG_ID+"'");
//		if(builder.SQL_Error_Code != 0){			
//			throw new Exception("ɾ����֯��Ϣ����ѯ�丸�ڵ��µĺ��ӽڵ����ʱ��������"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
//		}else{
//			if(count_linkman > 0){
//				throw new Exception("ɾ����֯��Ϣʱ����ԭ�򣺸���֯���Ѿ�¼����"+count_linkman+"��ϵ�ˣ����ȴ�����Щ��Ա���ٽ��б�������");
//			}
//		}
		
		int count_userinfo = builder.getCount("T_User_INFO", "Org_ID='"+org.ORG_ID+"'");
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("ɾ����֯��Ϣ����ѯ����֯�²���Ա��Ϣ��������"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}else{
			if(count_userinfo > 0){
				throw new Exception("ɾ����֯��Ϣʱ����ԭ�򣺸���֯���Ѿ�¼����"+count_userinfo+"������Ա�����ȴ�����Щ����Ա���ٽ��б�������");
			}
		}
		
		int count_terminal = builder.getCount("T_TERMINAL_INFO", "Hotel_ID='"+org.ORG_ID+"'");
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("ɾ����֯��Ϣ����ѯ�丸�ڵ��µĺ��ӽڵ����ʱ��������"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}else{
			if(count_terminal > 0){
				throw new Exception("ɾ����֯��Ϣʱ����ԭ�򣺸���֯���Ѿ�¼����"+count_terminal+"���նˣ����ȴ�����Щ�նˣ��ٽ��б�������");
			}
		}
		
		String sql_delete_org = "update T_ORG_INFO set Status='1' where ORG_ID = '"+org.ORG_ID+"' "+solution;
		builder.excute(sql_delete_org);		
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("ɾ����֯��Ϣ��������"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		OrgAdapter oa = new OrgAdapter();
		String xml = oa.getTreeXMLString(org.SUPERIOR_ORG_ID, Integer.parseInt(org.TOP_LEVEL)-1,solution);
		if(xml == null){
			throw new Exception("ɾ����֯��Ϣ��������֯�ڵ�("+org.ORG_ID+","+org.TOP_LEVEL+")���ݳ�������֯("+org.ORG_ID+","+org.TOP_LEVEL+")�����ڣ�");
		}else{
			logger.debug("��ѯɾ�������֯����"+xml);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,"ɾ����֯("+org.ORG_ID+"��"+org.ORG_NAME+")��Ϣ�ɹ���");
		reponse.set("xml", xml);
		return reponse;
	}
	
	private FlexReponse queryOrg(FlexRequest request) throws Exception{
		
		String solution = this.getSolution(request.type);
		
		DBBuilder builder = new DBBuilder();	
		T_ORG_INFO org = (T_ORG_INFO) request.getBean("org");	
		String sql_select_org = "select ORG_ID,ORG_NAME,ORG_TYPE,SUPERIOR_ORG_ID,TOP_LEVEL,HOTEL_GRADE,ADDRESS,ZIP_CODE,FAX_NO,CONTRACT_NO,CONTRACT_DATE,Website,ROOM_COUNT,AVERAGE_PRICE,SEND_SM_MAX_DAILY,STATUS,CREATE_USER_ID,CREATE_TIME,LAST_UPDATE_TIME,NOTES from T_ORG_INFO ";
		sql_select_org += " where ORG_ID='"+org.ORG_ID+"'"+" "+solution;
		Map<String,Object> map_org_selected = builder.queryMap(sql_select_org);
		if(builder.SQL_Error_Code != 0){
			throw new Exception("��ѯ��֯��Ϣ����ѯ�����ڵ㷢������ԭ��"+builder.SQL_Error_Code+","+builder.SQL_Error_Message);
		}
		
		OrgAdapter oa = new OrgAdapter();
		String xml = oa.getTreeXMLString(org.ORG_ID, Integer.parseInt(org.TOP_LEVEL),solution);
		if(xml == null){
			throw new Exception("��ѯ��֯��Ϣ��������֯�ڵ�("+org.ORG_ID+","+org.TOP_LEVEL+")���ݳ�������֯("+org.ORG_ID+","+org.TOP_LEVEL+")�����ڣ�");
		}else{
			logger.debug("��ѯ��֯����"+xml);
		}
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("map", map_org_selected);
		reponse.set("xml", xml);
		
//		if(org.ORG_TYPE.equals("2")){
//			String sql_hotel = "select ORG_ID,ORG_NAME,ORG_TYPE,SUPERIOR_ORG_ID,TOP_LEVEL,HOTEL_GRADE,ADDRESS,ZIP_CODE,FAX_NO,CONTRACT_NO,CONTRACT_DATE,Website,ROOM_COUNT,AVERAGE_PRICE,SEND_SM_MAX_DAILY,STATUS,CREATE_USER_ID,CREATE_TIME,LAST_UPDATE_TIME,NOTES from T_ORG_INFO ";
//			sql_hotel += "where SUPERIOR_ORG_ID='"+org.ORG_ID+"' "+solution;
//			List<Map<String,Object>> list_hotel = builder.queryList(sql_hotel);
//			if(builder.SQL_Error_Code != 0){
//				throw new Exception("��ѯ��֯��Ϣ����ǰ�����̣�"+org.ORG_ID+"��������Ͻ�Ƶ���Ϣ����ԭ��"+builder.SQL_Error_Code+","+builder.SQL_Error_Message);
//			}
//			reponse.set("list", list_hotel);
//		}
		
		return reponse;
	}
	
	private FlexReponse queryCommonOrg(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();	
		T_ORG_INFO org = (T_ORG_INFO) request.getBean("org");	
		
		String sql = "select ORG_ID,ORG_NAME,ORG_TYPE,SUPERIOR_ORG_ID,TOP_LEVEL,HOTEL_GRADE,ADDRESS,ZIP_CODE,FAX_NO,CONTRACT_NO,CONTRACT_DATE,Website,ROOM_COUNT,AVERAGE_PRICE,SEND_SM_MAX_DAILY,STATUS,CREATE_USER_ID,CREATE_TIME,LAST_UPDATE_TIME,NOTES from T_ORG_INFO ";
		sql += " where ORG_TYPE in ('0','2','3','4') ";
		boolean haswhere = false;
		haswhere = true;
		if(org.ORG_TYPE != null && org.ORG_TYPE.trim().length()>0){
			if(haswhere){
				sql += " and ORG_TYPE = '"+org.ORG_TYPE+"' ";
			}else{
				sql += " where ORG_TYPE = "+org.ORG_TYPE+" ";
				haswhere = true;
			}
		}
		
		if(org.ORG_ID != null && org.ORG_ID.trim().length()>0){
			if(haswhere){
				sql += " and ORG_ID = '"+org.ORG_ID+"' ";
			}else{
				sql += " where ORG_ID = '"+org.ORG_ID+"' ";
				haswhere = true;
			}			
		}
		
		if(org.ORG_NAME != null && org.ORG_NAME.trim().length()>0){
			if(haswhere){
				sql += " and ORG_NAME = '"+org.ORG_NAME+"' ";
			}else{
				sql += " where ORG_NAME = '"+org.ORG_NAME+"' ";
				haswhere = true;
			}			
		}
		
		if(org.HOTEL_GRADE != null && org.HOTEL_GRADE.trim().length()>0){
			if(haswhere){
				sql += " and HOTEL_GRADE = "+org.HOTEL_GRADE+" ";
			}else{
				sql += " where HOTEL_GRADE = "+org.HOTEL_GRADE+" ";
				haswhere = true;
			}			
		}
		
		if(org.STATUS != null && org.STATUS.trim().length()>0){
			if(haswhere){
				sql += " and STATUS = "+org.STATUS+" ";
			}else{
				sql += " where STATUS = "+org.STATUS+" ";
				haswhere = true;
			}			
		}		
		sql += " order by Create_Time ";
		
		List<Map<String,Object>> list = builder.queryList(sql);
		if(builder.SQL_Error_Code != 0){
			throw new Exception("��ѯ��֯��Ϣ��ͨ�÷�ʽ��ѯ���ִ���ԭ��"+builder.SQL_Error_Code+","+builder.SQL_Error_Message);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list", list);		
		return reponse;
	}
	
	private FlexReponse initOrgTreeNode(FlexRequest request) throws Exception{

		String solution = this.getSolution(request.type);
		
		OrgAdapter oa = new OrgAdapter();
		String xml = oa.getTreeXMLString("HAPPY", 0,solution);
		if(xml == null){
			throw new Exception("��ʼ�����ڵ�(\"HAPPY\", 0)����");
		}else{
			logger.debug("��ʼ��֯����"+xml);
		}
		
		DBBuilder builder = new DBBuilder();
		List<Map<String,Object>> list = builder.queryList("select Parm_Key,Parm_Values from T_Public_Parm_Info where Parm_Key in ('Reward_SM','Reward_HIRE','Reward_FEE','Reward_IPFEE')");
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("xml", xml);
		reponse.set("list", list);
		return reponse;
	}
	
	private String getSolution(int type){
		String solution = "";
		if(type == 0){
			solution = " and Status='0' and Org_Type in ('0','2','3','4')";
		}else if(type == 1){
			solution = " and Status='1' and Org_Type in ('0','2','3','4')";
		}else{
			solution = " and Org_Type in ('0','2','3','4')";
		}
		return solution;
	}
	
	private FlexReponse addDocument(FlexRequest request)throws Exception{
		T_ORG_DOCUMENT document = (T_ORG_DOCUMENT) request.getBean("document");
		DBFactory factory = new DBFactory();
		factory.openTransaction();
		if(factory.SQL_Error_Code != 0){
			throw new Exception("�����ĵ�������ѯԭʼ�ĵ���������ԭ��"+factory.SQL_Error_Code+","+factory.SQL_Error_Message);
		}
		int count_id = factory.getCount("T_ORG_DOCUMENT", "DOCUMENT_TYPE='"+document.DOCUMENT_TYPE+"' and DOCUMENT_NAME='"+document.DOCUMENT_NAME+"'");
		if(factory.SQL_Error_Code != 0){
			throw new Exception("�����ĵ�������ѯ��ǰ¼���ļ���������ԭ��"+factory.SQL_Error_Code+","+factory.SQL_Error_Message);
		}
		if(count_id>0){
			factory.interruptTransaction();
			throw new Exception("�����ĵ���������"+document.DOCUMENT_TYPE+"������"+document.DOCUMENT_NAME+"�Ѿ����ڣ�");
		}
		//document.DOCUMENT_PATH += String.valueOf(nex_seq);
		String sql_add = "insert into T_ORG_DOCUMENT (ID,Org_ID,Document_Type,Document_Path,Document_Name,Create_User_ID,Create_Time,Notes) values ";
		sql_add += "(Five_Bit_SEQ.nextVal,'"+document.ORG_ID+"','"+document.DOCUMENT_TYPE+"','"+document.DOCUMENT_PATH+"','"+document.DOCUMENT_NAME+"','"+request.user_id+"',sysdate,'"+document.NOTES+"')";
		File file = new File(document.DOCUMENT_PATH);
		if(file.exists()){
			factory.excute(sql_add);
			if(factory.SQL_Error_Code != 0){
				throw new Exception("�����ĵ�������ӵ����ĵ���"+document.DOCUMENT_TYPE+"��"+document.DOCUMENT_NAME+"������ԭ��"+factory.SQL_Error_Code+","+factory.SQL_Error_Message);
			}
		}else{
			factory.interruptTransaction();
			throw new Exception("�����ĵ�������ӵ����ĵ������Ҳ����ļ�"+document.DOCUMENT_PATH);
		}
		
		factory.closeTransaction();
		if(factory.SQL_Error_Code != 0){
			throw new Exception("�����ĵ������ύ��ӵ����ĵ���"+document.DOCUMENT_TYPE+"��"+document.DOCUMENT_NAME+"���������ԭ��"+factory.SQL_Error_Code+","+factory.SQL_Error_Message);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,"��ӵ����ĵ���"+document.DOCUMENT_TYPE+"��"+document.DOCUMENT_NAME+"���ɹ���");	
		return reponse;
	}
	
	private FlexReponse updateDocument(FlexRequest request)throws Exception{
		T_ORG_DOCUMENT document = (T_ORG_DOCUMENT) request.getBean("document");
		DBFactory factory = new DBFactory();
		factory.openTransaction();
		int count_id = factory.getCount("T_ORG_DOCUMENT", "ID='"+document.ID+"'");
		if(factory.SQL_Error_Code != 0){
			throw new Exception("�����ĵ�������ѯԭʼ�ĵ���������ԭ��"+factory.SQL_Error_Code+","+factory.SQL_Error_Message);
		}
		
		if(count_id == 0){
			factory.interruptTransaction();
			throw new Exception("�����ĵ������滻�����ĵ�����"+document.ID+"�����ڣ�");
		}
		//document.DOCUMENT_PATH += document.ID;
		String sql_update = "update T_ORG_DOCUMENT set ";
		sql_update += " DOCUMENT_PATH='"+document.DOCUMENT_PATH+"' , ";
		sql_update += " DOCUMENT_NAME='"+document.DOCUMENT_NAME+"' , ";
		sql_update += "LAST_UPDATE_TIME	= sysdate,";
		sql_update += " NOTES='"+document.NOTES+"'  ";
		sql_update += " where ID = "+document.ID;
		File file = new File(document.DOCUMENT_PATH);
		if(file.exists()){
			int update_count = factory.excute(sql_update);
			if(factory.SQL_Error_Code != 0){
				throw new Exception("�����ĵ������滻�����ĵ���"+document.ID+"������ԭ��"+factory.SQL_Error_Code+","+factory.SQL_Error_Message);
			}else{
				if(update_count != 1){
					factory.interruptTransaction();
					throw new Exception("�����ĵ������滻�����ĵ�����"+document.ID+"��Ψһ��");
				}
			}
		}else{
			factory.interruptTransaction();
			throw new Exception("�����ĵ������滻�����ĵ������Ҳ����ļ�"+document.DOCUMENT_PATH);
		}

		factory.closeTransaction();
		if(factory.SQL_Error_Code != 0){
			throw new Exception("�����ĵ������滻�����ĵ���"+document.DOCUMENT_TYPE+"��"+document.DOCUMENT_NAME+"������ԭ��"+factory.SQL_Error_Code+","+factory.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,"�滻�����ĵ���"+document.DOCUMENT_TYPE+"��"+document.DOCUMENT_NAME+"���ɹ���");	
		return reponse;
	}

	private FlexReponse deleteDocument(FlexRequest request)throws Exception{
		T_ORG_DOCUMENT document = (T_ORG_DOCUMENT) request.getBean("document");
		DBFactory factory = new DBFactory();
		factory.openTransaction();
		String sql = "select DOCUMENT_PATH from T_Org_Document where ID = "+document.ID;
		Map<String,Object> map = factory.queryMap(sql);
		if(factory.SQL_Error_Code != 0){
			throw new Exception("�����ĵ�������ѯԭʼ�ĵ���������ԭ��"+factory.SQL_Error_Code+","+factory.SQL_Error_Message);
		}else{
			if(map == null){
				factory.interruptTransaction();
				throw new Exception("�����ĵ�������ȡɾ�������ĵ�����"+document.ID+"·����Ϣʧ�ܣ����ݿ�û�д洢��ֵ��");
			}
		}
		
		String DOCUMENT_PATH = (String) map.get("DOCUMENT_PATH");
		
		String sql_delete = "delete from T_ORG_DOCUMENT ";
		sql_delete += " where ID = "+document.ID;
		File file = new File(DOCUMENT_PATH);
		if(file.exists()){
			int delete_count = factory.excute(sql_delete);
			if(factory.SQL_Error_Code != 0){
				throw new Exception("�����ĵ�����ɾ�������ĵ���"+document.DOCUMENT_TYPE+"��"+document.DOCUMENT_NAME+"������ԭ��"+factory.SQL_Error_Code+","+factory.SQL_Error_Message);
			}else{
				if(delete_count != 1){
					factory.interruptTransaction();
					throw new Exception("�����ĵ�����ɾ�������ĵ�����"+document.ID+"��Ψһ��");
				}
			}
			file.delete();
		}else{
			factory.interruptTransaction();
			throw new Exception("�����ĵ�����ɾ�������ĵ������Ҳ����ļ�"+document.DOCUMENT_PATH);
		}

		factory.closeTransaction();
		if(factory.SQL_Error_Code != 0){
			throw new Exception("�����ĵ������滻�����ĵ���"+document.DOCUMENT_TYPE+"��"+document.DOCUMENT_NAME+"������ԭ��"+factory.SQL_Error_Code+","+factory.SQL_Error_Message);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,"ɾ�������ĵ��ɹ���");	
		return reponse;
	}

	private FlexReponse queryDocument(FlexRequest request)throws Exception{
		T_ORG_DOCUMENT document = (T_ORG_DOCUMENT) request.getBean("document");
		String notbefore = (String) request.getBean("notbefore");
		String notafter = (String) request.getBean("notafter");
		
		DBBuilder builder = new DBBuilder();
		String sql = "select ID,Org_ID,Document_Type,Document_Path,Document_Name,Create_User_ID,Create_Time,Last_Update_Time,Notes from T_Org_Document  ";
		sql += " where CREATE_TIME >= to_date('"+notbefore+"','yyyy-mm-dd hh24:mi:ss') and CREATE_TIME <= to_date('"+notafter+"','yyyy-mm-dd hh24:mi:ss') ";
		
		if(document.DOCUMENT_TYPE != null && document.DOCUMENT_TYPE.trim().length()>0){
			sql += " and DOCUMENT_TYPE = '"+document.DOCUMENT_TYPE+"' ";
		}
		if(document.DOCUMENT_NAME != null && document.DOCUMENT_NAME.trim().length()>0){
			sql += " and DOCUMENT_NAME = '"+document.DOCUMENT_NAME+"' ";
		}
		if(document.ORG_ID != null && document.ORG_ID.trim().length()>0){
			sql += " and ORG_ID = '"+document.ORG_ID+"' ";
		}
		sql += " order by Create_Time";
		List<Map<String,Object>> list = builder.queryList(sql);
		if(builder.SQL_Error_Code != 0){
			throw new Exception("�����ĵ�������ѯԭʼ�ĵ���������ԭ��"+builder.SQL_Error_Code+","+builder.SQL_Error_Message);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,null);	
		reponse.set("list", list);
		return reponse;
	}
	
	private FlexReponse viewDocument(FlexRequest request)throws Exception{
		T_ORG_DOCUMENT document = (T_ORG_DOCUMENT) request.getBean("document");
		String sql = "select DOCUMENT_PATH from T_Org_Document where ID = "+document.ID;
		DBBuilder builder = new DBBuilder();
		Map<String,Object> map = builder.queryMap(sql);
		if(builder.SQL_Error_Code != 0){
			throw new Exception("�����ĵ�������ѯԭʼ�ĵ���������ԭ��"+builder.SQL_Error_Code+","+builder.SQL_Error_Message);
		}else{
			if(map == null){
				throw new Exception("�����ĵ�������ȡɾ�������ĵ�����"+document.ID+"·����Ϣʧ�ܣ����ݿ�û�д洢��ֵ��");
			}
		}
		
		String banse_url = (String) map.get("DOCUMENT_PATH");
		File file = new File(banse_url);		
		String visit_url = "";
		if(file.exists()){
			FileUtil fu = new FileUtil();
			if(fu.zipFile(banse_url, ChobitsConfig.TempFileForderPath, document.ID+".zip")){
				visit_url = ChobitsConfig.VisitURL+"" + "/TempFile/" + document.ID+".zip";
			}else{
				throw new Exception("�ļ�"+banse_url+"�Ѿ�ѹ��ʧ�ܣ��������Ա��ϵ");
			}	
		}else{
			throw new Exception("�ļ�"+banse_url+"�Ѿ���ʧ���������Ա��ϵ");
		}
		FlexReponse reponse = new FlexReponse(request,0,null);	
		reponse.set("url", visit_url);
		return reponse;
	}
	
	 private FlexReponse initDocument(FlexRequest request)throws Exception{
//		 0ƽ̨,1�ն˳���,2������,3�Ƶ�,4���ҵ��
		ParamHandler handler = new ParamHandler();
		List<Map<String,Object>> list_org_edit = handler.getT_Org_Info_Org_ID_Org_Name("('0','2','3','4')", "('0')");
		FlexReponse reponse = new FlexReponse(request,0,null);	
		reponse.set("list_org_edit", list_org_edit);
		return reponse;
	 }
}
