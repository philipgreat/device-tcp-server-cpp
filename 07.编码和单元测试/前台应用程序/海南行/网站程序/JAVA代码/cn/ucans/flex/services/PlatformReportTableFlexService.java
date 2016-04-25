package cn.ucans.flex.services;

import java.io.File;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import chobits.db.DBBuilder;
import chobits.db.DBFactory;
import chobits.log.Logger;
import cn.ucans.adapter.OrgAdapter;
import cn.ucans.bean.T_ORG_DOCUMENT;
import cn.ucans.bean.T_ORG_INFO;
import cn.ucans.flex.FlexReponse;
import cn.ucans.flex.FlexRequest;
import cn.ucans.param.ParamHandler;

public class PlatformReportTableFlexService extends FlexService{
	
	private Logger logger = new Logger(PlatformReportTableFlexService.class.getName());
	
	public FlexReponse doTransaction(FlexRequest request){
		logger.debug("-----------------------------ƽ̨���뱨������ ����-------------------------------");
		FlexReponse reponse = null;
		
		try{			
			super.checkRole(request);
			
			int ope_id = request.ope_id;			
			switch(ope_id){
				case 1500:
					reponse = this.initForm(request);
					break;
				case 1501:
					reponse = this.doQuery(request);
					break;
				case 1502:
					reponse = this.doLead(request);
					break;
				default:
					break;
			}
			logger.debug("-----------------------------ƽ̨���뱨������ �ɹ����-------------------------------");
		}catch(Exception e){
			logger.error(e.getMessage());
			reponse = new FlexReponse(request,-1,e.getMessage());	
			logger.debug("-----------------------------ƽ̨���뱨������ ִ��ʧ��-------------------------------");
		}
		
		return reponse;
	}
	
	private FlexReponse initForm(FlexRequest request) throws Exception{	
//		private var collection_list_hotel_edit:ArrayCollection = null;
//	private var collection_list_broker_edit:ArrayCollection = null;
//	
//	private var collection_list_hotel_query:ArrayCollection = null;
//	private var collection_list_broker_query:ArrayCollection = null;
		Map<String,Object> map = new HashMap<String,Object>();
		map.put("label", "ȫ��");
		map.put("data", "");
		ParamHandler handler = new ParamHandler();
		List<Map<String,Object>> list_hotel_edit = handler.getT_Org_Info_Org_ID_Org_Name("(3)", "('0','1')");
		List<Map<String,Object>> list_hotel_query = handler.getT_Org_Info_Org_ID_Org_Name("(3)", "('0','1')");
		List<Map<String,Object>> list_broker_edit = handler.getT_Org_Info_Org_ID_Org_Name("(2)", "('0','1')");
		List<Map<String,Object>> list_broker_query = handler.getT_Org_Info_Org_ID_Org_Name("(2)", "('0','1')");
		list_hotel_query.add(0, map);
		list_broker_query.add(0, map);
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list_hotel_edit", list_hotel_edit);
		reponse.set("list_hotel_query", list_hotel_query);
		reponse.set("list_broker_edit", list_broker_edit);
		reponse.set("list_broker_query", list_broker_query);
		return reponse;
	}
	
	private FlexReponse doQuery(FlexRequest request) throws Exception{
		String sql = (String) request.getBean("sql");
		String solution = (String) request.getBean("solution");
		DBBuilder builder = new DBBuilder();
		List<Map<String,Object>> list = builder.queryList(sql);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("ƽ̨����ͳ����Ϣ��ԭ��"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}	
		
		String sql_user = "select User_Name from T_User_Info where User_ID='"+request.user_id+"'";
		Map<String,Object> map_user = builder.queryMap(sql_user);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("ƽ̨������ѯ����Ա������ԭ��"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}else{
			if(map_user == null){
				throw new Exception("��ȡ����Ա����ʧ�ܣ��Ҳ������Ϊ"+request.user_id+"�Ĳ���Ա��Ϣ��");
			}
		}
		String UserName = (String) map_user.get("USER_NAME");
		String sql_org = "select Org_Name from T_Org_Info where Org_ID='"+request.org_id+"'";
		Map<String,Object> map_org = builder.queryMap(sql_org);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("ƽ̨������ѯ����Ա������ԭ��"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}else{
			if(map_user == null){
				throw new Exception("��ȡ����Ա������֯���ƣ��Ҳ������Ϊ"+request.org_id+"����֯��Ϣ��");
			}
		}
		String OrgName = (String) map_org.get("ORG_NAME");
		
		String sql_sum = "select sum(Real_Pay_Cash) as REAL_PAY_CASH from V_Service_Cash where "+solution;
		Map<String,Object> map_sum = builder.queryMap(sql_sum);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("ƽ̨������ѯ�����ܽ�ԭ��"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}else{
			if(map_user == null){
				throw new Exception("��ȡ�ܽ��ʧ�ܣ�");
			}
		}
		String sum_all = (String)map_sum.get("REAL_PAY_CASH");
//		Map<String,Object> map_null = new HashMap<String,Object>();
//		map_null.put("BROKER_ID", "");
//		map_null.put("HOTEL_ID", "");
//		map_null.put("REAL_PAY_CASH", "");
//		map_null.put("CREATE_TIME", "");
//		list.add(map_null);
//		list.add(map_null);
//		list.add(map_null);
		if(list != null && list.size()>0){
			Map<String,Object> map_all_sum = new HashMap<String,Object>();
			map_all_sum.put("BROKER_ID", "�ܼƣ�");
			map_all_sum.put("HOTEL_ID", "");
			map_all_sum.put("REAL_PAY_CASH", sum_all);
			map_all_sum.put("CREATE_TIME", "");
			list.add(map_all_sum);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list", list);
		reponse.set("OrgName", UserName);
		reponse.set("UserName", OrgName);
		
		return reponse;
	}

	private FlexReponse doLead(FlexRequest request) throws Exception{
		String xml = (String) request.getBean("xml");
		DBBuilder builder = new DBBuilder();
		String url = builder.leadDataToFile(xml, "ReportData");
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("ƽ̨�����������ݣ�ԭ��"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}	
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("url", url);
		return reponse;
	}
}
