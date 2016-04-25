package cn.ucans.param;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import cn.ucans.bean.T_INFO_NUMBER_OBJECT;

import chobits.db.DBBuilder;
import chobits.log.Logger;

public class ParamHandler {
	
	private DBBuilder builder =null;
	
	public int Error_Code = -1;
	public String Error_Message = "";
	
	private Logger logger = new Logger(ParamHandler.class.getName());
	
	public ParamHandler() {
		builder = new DBBuilder();
	}
	/**
	 * 获取角色下拉列表
	 */
	public List<Map<String,Object>> getT_Role_Info__Role_ID(String type){
		this.clear();
		String sql = "select distinct Role_ID,Role_Name from T_Role_Info";
		Connection connection = null;
		PreparedStatement statement = null;
		ResultSet resultset = null;
		List<Map<String,Object>> list = new ArrayList<Map<String,Object>>();
		try{
			connection = builder.getConnection();
			logger.debug(sql);
			statement = connection.prepareStatement(sql);
			resultset = statement.executeQuery();
			while(resultset.next()){
				Map<String,Object> map = new HashMap<String,Object>();
				String label = builder.trackValue(resultset.getObject("Role_Name"));
				String data = builder.trackValue(resultset.getObject("Role_ID"));
				map.put("label", label);
				map.put("data", data);
				list.add(map);
			}
			builder.close(connection, statement, resultset);
		}catch(Exception e){
			builder.close(connection, statement, resultset);
			builder.trackException(e);
			this.setError(-900001, "参数初始化，获取角色id信息列表发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		return list;
	}
	
	public List<Map<String,Object>> getPhoneFromRoomNO(String room_no,String org_id){
		this.clear();
		String sql = "";
		if(room_no == null){
			sql = "select distinct Msisdn from T_Terminal_Info where Hotel_ID='"+org_id+"'";
		}else{
			sql = "select distinct Msisdn from T_Terminal_Info where Hotel_ID='"+org_id+"' and Room_NO='"+room_no+"'";
		}
		
		Connection connection = null;
		PreparedStatement statement = null;
		ResultSet resultset = null;
		List<Map<String,Object>> list = new ArrayList<Map<String,Object>>();
		try{
			connection = builder.getConnection();
			logger.debug(sql);
			statement = connection.prepareStatement(sql);
			resultset = statement.executeQuery();
			while(resultset.next()){
				Map<String,Object> map = new HashMap<String,Object>();
				String label = builder.trackValue(resultset.getObject("MSISDN"));
				String data = builder.trackValue(resultset.getObject("MSISDN"));
				map.put("label", label);
				map.put("data", data);
				list.add(map);
			}
			builder.close(connection, statement, resultset);
		}catch(Exception e){
			builder.close(connection, statement, resultset);
			builder.trackException(e);
			this.setError(-900001, "参数初始化，指定房间里存放终端手机号码信息列表发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		return list;
	}
	
	/**
	 * 获取终端状态下拉列表
	 */
	public List<Map<String,Object>> getT_Terminal_Info_Status(String field,String At_Hotel_Flag){
		this.clear();
		String sql = "";
		if(field == null){
			if(At_Hotel_Flag == null){
				sql = "select distinct Terminal_Status,Notes from T_TERMINAL_STATUS ";
			}else{
				sql = "select distinct Terminal_Status,Notes from T_TERMINAL_STATUS where At_Hotel_Flag in "+At_Hotel_Flag;
			}
		}else{
			if(At_Hotel_Flag == null){
				sql = "select distinct Terminal_Status,Notes from T_TERMINAL_STATUS where Terminal_Status in "+field;
			}else{
				sql = "select distinct Terminal_Status,Notes from T_TERMINAL_STATUS where Terminal_Status in "+field+" and At_Hotel_Flag in "+At_Hotel_Flag;
			}			
		}
		Connection connection = null;
		PreparedStatement statement = null;
		ResultSet resultset = null;
		List<Map<String,Object>> list = new ArrayList<Map<String,Object>>();
		try{
			connection = builder.getConnection();
			logger.debug(sql);
			statement = connection.prepareStatement(sql);
			resultset = statement.executeQuery();
			while(resultset.next()){
				Map<String,Object> map = new HashMap<String,Object>();
				String label = builder.trackValue(resultset.getObject("Notes"));
				String data = builder.trackValue(resultset.getObject("Terminal_Status"));
				map.put("label", label);
				map.put("data", data);
				list.add(map);
			}
			builder.close(connection, statement, resultset);
		}catch(Exception e){
			builder.close(connection, statement, resultset);
			builder.trackException(e);
			this.setError(-900001, "参数初始化，获取角色id信息列表发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		return list;
	}
	//查找终端型号下拉列表数据
	public List<Map<String,Object>> getT_Terminal_Type(){
		this.clear();
		String sql = "select distinct Org_ID,Terminal_Type,Terminal_Name from T_Terminal_Type";
		Connection connection = null;
		PreparedStatement statement = null;
		ResultSet resultset = null;
		List<Map<String,Object>> list = new ArrayList<Map<String,Object>>();
		try{
			connection = builder.getConnection();
			logger.debug(sql);
			statement = connection.prepareStatement(sql);
			resultset = statement.executeQuery();
			while(resultset.next()){
				Map<String,Object> map = new HashMap<String,Object>();
				String label = builder.trackValue(resultset.getObject("Terminal_Name"));
				String data = builder.trackValue(resultset.getObject("Terminal_Type"));
				map.put("label", label);
				map.put("data", data);
				list.add(map);
			}
			builder.close(connection, statement, resultset);
		}catch(Exception e){
			builder.close(connection, statement, resultset);
			builder.trackException(e);
			this.setError(-900001, "参数初始化，获取角色id信息列表发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		return list;
	}
	/*
	 *根据指定的field和Status值确定要查询的组织机构范围，例如：field= "('0','2','3')" 
	 * */
	public List<Map<String,Object>> getT_Org_Info_Org_ID_Org_Name(String field,String status){
		this.clear();
		String sql = "select distinct Org_ID,Org_Name from T_Org_Info where Org_Type in "+field +" and Status in "+status;
		Connection connection = null;
		PreparedStatement statement = null;
		ResultSet resultset = null;
		List<Map<String,Object>> list = new ArrayList<Map<String,Object>>();
		try{
			connection = builder.getConnection();
			logger.debug(sql);
			statement = connection.prepareStatement(sql);
			resultset = statement.executeQuery();
			while(resultset.next()){
				Map<String,Object> map = new HashMap<String,Object>();
				String label = builder.trackValue(resultset.getObject("Org_Name"));
				String data = builder.trackValue(resultset.getObject("Org_ID"));
				map.put("label", label);
				map.put("data", data);
				list.add(map);
			}
			builder.close(connection, statement, resultset);
		}catch(Exception e){
			builder.close(connection, statement, resultset);
			builder.trackException(e);
			this.setError(-900001, "参数初始化，获取角色id信息列表发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		return list;
	}
	
	/*
	 *获取T_User_Info表中的独立的id编号 
	 * */
	public List<Map<String,Object>> getT_User_Info_User_ID(){
		this.clear();
		String sql = "select distinct User_ID from T_User_Info";
		Connection connection = null;
		PreparedStatement statement = null;
		ResultSet resultset = null;
		List<Map<String,Object>> list = new ArrayList<Map<String,Object>>();
		try{
			connection = builder.getConnection();
			logger.debug(sql);
			statement = connection.prepareStatement(sql);
			resultset = statement.executeQuery();
			while(resultset.next()){
				Map<String,Object> map = new HashMap<String,Object>();
				String value = builder.trackValue(resultset.getObject("User_ID"));
				map.put("label", value);
				map.put("data", value);
				list.add(map);
			}
			builder.close(connection, statement, resultset);
		}catch(Exception e){
			builder.close(connection, statement, resultset);
			builder.trackException(e);
			this.setError(-900001, "参数初始化，获取角色id信息列表发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		return list;
	}
	
	public List<Map<String,Object>> getT_T_Area_code_Param__Area_Code(String type){
		this.clear();
		String sql = "select distinct Area_Code,Area_Name from T_AREA_CODE_PARM";
		Connection connection = null;
		PreparedStatement statement = null;
		ResultSet resultset = null;
		List<Map<String,Object>> list = new ArrayList<Map<String,Object>>();
		try{
			connection = builder.getConnection();
			logger.debug(sql);
			statement = connection.prepareStatement(sql);
			resultset = statement.executeQuery();
			while(resultset.next()){
				Map<String,Object> map = new HashMap<String,Object>();
				String label = builder.trackValue(resultset.getObject("Area_Name"));
				String data = builder.trackValue(resultset.getObject("Area_Code"));
				map.put("label", label);
				map.put("data", data);
				list.add(map);
			}
			builder.close(connection, statement, resultset);
		}catch(Exception e){
			builder.close(connection, statement, resultset);
			builder.trackException(e);
			this.setError(-900001, "参数初始化，获取角色id信息列表发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		return list;
	}
	
	/**
	 * 根据组织类型，获取下拉列表数据源
	 * @param type 组织类型(0平台,1终端厂家,2代销商,3酒店,4广告业主)
	 * @return
	 */
	public List<Map<String,Object>> getT_OrgID__OrgName(String type){
		this.clear();
		String sql = "select ORG_ID,ORG_NAME from T_ORG_INFO where  Status='0'";
		if(type!=null&&type.length()>0){
			sql+="  and  Org_Type='" + type+"'";
		}
		Connection connection = null;
		PreparedStatement statement = null;
		ResultSet resultset = null;
		List<Map<String,Object>> list = new ArrayList<Map<String,Object>>();
		try{
			connection = builder.getConnection();
			logger.debug(sql);
			statement = connection.prepareStatement(sql);
			resultset = statement.executeQuery();
			while(resultset.next()){
				Map<String,Object> map = new HashMap<String,Object>();
				String label = resultset.getString("ORG_NAME").toString();
				String data = resultset.getString("ORG_ID").toString();
				map.put("label", label);
				map.put("data", data);
				list.add(map);
			}
			builder.close(connection, statement, resultset);
		}catch(Exception e){
			builder.close(connection, statement, resultset);
			builder.trackException(e);
			this.setError(-900001, "参数初始化，获取终端厂家信息列表发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		return list;
	}
	
	//------------
	
	public List<Map<String,Object>> get_T_Org_Info(String type){
		this.clear();
		String sql = "select distinct Org_ID,Org_Name from T_Org_Info where Org_Type='3' and Status='0'";
		Connection connection = null;
		PreparedStatement statement = null;
		ResultSet resultset = null;
		List<Map<String,Object>> list = new ArrayList<Map<String,Object>>();
		try{
			connection = builder.getConnection();
			logger.debug(sql);
			statement = connection.prepareStatement(sql);
			resultset = statement.executeQuery();
			Map<String,Object> map1 = new HashMap<String,Object>();
			map1.put("label", "全部");
			map1.put("data", "");
			list.add(map1);
			while(resultset.next()){
				Map<String,Object> map = new HashMap<String,Object>();
				String label = builder.trackValue(resultset.getObject("Org_Name"));
				String data = builder.trackValue(resultset.getObject("Org_ID"));
				map.put("label", label);
				map.put("data", data);
				list.add(map);
			}
			builder.close(connection, statement, resultset);
		}catch(Exception e){
			builder.close(connection, statement, resultset);
			builder.trackException(e);
			this.setError(-900001, "参数初始化，获取信息列表发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		return list;
	}
	public List<Map<String,Object>> get_T_Black_Type(String type){
		this.clear();
		String sql = "select Black_Type,Black_Name from T_Black_Type";
		Connection connection = null;
		PreparedStatement statement = null;
		ResultSet resultset = null;
		List<Map<String,Object>> list = new ArrayList<Map<String,Object>>();
		try{
			connection = builder.getConnection();
			logger.debug(sql);
			statement = connection.prepareStatement(sql);
			resultset = statement.executeQuery();
			while(resultset.next()){
				Map<String,Object> map = new HashMap<String,Object>();
				String label = builder.trackValue(resultset.getObject("Black_Name"));
				String data = builder.trackValue(resultset.getObject("Black_Type"));
				map.put("label", label);
				map.put("data", data);
				list.add(map);
			}
			builder.close(connection, statement, resultset);
		}catch(Exception e){
			builder.close(connection, statement, resultset);
			builder.trackException(e);
			this.setError(-900001, "参数初始化，获取信息列表发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		return list;
	}
	private void setError(int Error_Code,String Error_Message){
		this.Error_Code = Error_Code;
		this.Error_Message = Error_Message;
		logger.error(Error_Message);
	}
	private void clear(){
		this.Error_Code = 0;
		this.Error_Message = "";
	}
	
	public List<Map<String,Object>> get_T_Org_InfoNoAll(String type){
		this.clear();
		String sql = "select distinct Org_ID,Org_Name from T_Org_Info where Org_Type='3' and Status='0'";
		Connection connection = null;
		PreparedStatement statement = null;
		ResultSet resultset = null;
		List<Map<String,Object>> list = new ArrayList<Map<String,Object>>();
		try{
			connection = builder.getConnection();
			logger.debug(sql);
			statement = connection.prepareStatement(sql);
			resultset = statement.executeQuery();
			Map<String,Object> map1 = new HashMap<String,Object>();
			while(resultset.next()){
				Map<String,Object> map = new HashMap<String,Object>();
				String label = builder.trackValue(resultset.getObject("Org_Name"));
				String data = builder.trackValue(resultset.getObject("Org_ID"));
				map.put("label", label);
				map.put("data", data);
				list.add(map);
			}
			builder.close(connection, statement, resultset);
		}catch(Exception e){
			builder.close(connection, statement, resultset);
			builder.trackException(e);
			this.setError(-900001, "参数初始化，获取信息列表发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		return list;
	}
	
	public List<Map<String,Object>> getLanguageName(String type){
		this.clear();
		String sql = "select Language,Language_Name from T_Info_Language_List";
		Connection connection = null;
		PreparedStatement statement = null;
		ResultSet resultset = null;
		List<Map<String,Object>> list = new ArrayList<Map<String,Object>>();
		try{
			connection = builder.getConnection();
			logger.debug(sql);
			statement = connection.prepareStatement(sql);
			resultset = statement.executeQuery();
			Map<String,Object> map1 = new HashMap<String,Object>();
			while(resultset.next()){
				Map<String,Object> map = new HashMap<String,Object>();
				String label = builder.trackValue(resultset.getObject("Language_Name"));
				String data = builder.trackValue(resultset.getObject("Language"));
				map.put("label", label);
				map.put("data", data);
				list.add(map);
			}
			builder.close(connection, statement, resultset);
		}catch(Exception e){
			builder.close(connection, statement, resultset);
			builder.trackException(e);
			this.setError(-900001, "参数初始化，获取信息列表发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		return list;
	}
	public List<Map<String,Object>> getT_Org_InfoHoteName(ArrayList list1){
		this.clear();
		String Hotel_Grade_data = "";
		for(int i = 0;i<list1.size();i++){
			Map map = (Map)list1.get(i);	
			Hotel_Grade_data +=map.get("data")+",";
		}
		Hotel_Grade_data =  Hotel_Grade_data.substring(0, Hotel_Grade_data.length()-1);
		logger.debug(Hotel_Grade_data);
		String sql = "select Org_ID,Org_Name from T_Org_Info where Org_Type ='3' and Status='0' and Hotel_Grade in("+Hotel_Grade_data+")";	
		System.out.println(sql);
		Connection connection = null;
		PreparedStatement statement = null;
		ResultSet resultset = null;
		List<Map<String,Object>> list = new ArrayList<Map<String,Object>>();
		try{
			connection = builder.getConnection();
			logger.debug(sql);
			statement = connection.prepareStatement(sql);
			resultset = statement.executeQuery();
			Map<String,Object> map1 = new HashMap<String,Object>();
			while(resultset.next()){
				Map<String,Object> map = new HashMap<String,Object>();
				String label = builder.trackValue(resultset.getObject("Org_Name"));
				String data = builder.trackValue(resultset.getObject("Org_ID"));
				map.put("label", label);
				map.put("data", data);
				list.add(map);
			}
			builder.close(connection, statement, resultset);
		}catch(Exception e){
			builder.close(connection, statement, resultset);
			builder.trackException(e);
			this.setError(-900001, "参数初始化，获取信息列表发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		return list;
	}
	public List<Map<String,Object>> getT_Info_Number_Object(String type){
		this.clear();
		String sql = "select Send_Group_ID,Send_Group_Name from T_Info_Number_Object";
	
		Connection connection = null;
		PreparedStatement statement = null;
		ResultSet resultset = null;
		List<Map<String,Object>> list = new ArrayList<Map<String,Object>>();
		try{
			connection = builder.getConnection();
			logger.debug(sql);
			statement = connection.prepareStatement(sql);
			resultset = statement.executeQuery();
			while(resultset.next()){
				Map<String,Object> map = new HashMap<String,Object>();
				String label = builder.trackValue(resultset.getObject("Send_Group_Name"));
				String data = builder.trackValue(resultset.getObject("Send_Group_ID"));
				map.put("label", label);
				map.put("data", data);
				list.add(map);
			}
			builder.close(connection, statement, resultset);
		}catch(Exception e){
			builder.close(connection, statement, resultset);
			builder.trackException(e);
			this.setError(-900001, "参数初始化，获取角色id信息列表发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		return list;
	}
	public List<Map<String,Object>> getT_Info_Type(String type){
		this.clear();
		String sql = "select Info_Type,Info_Name from T_Info_Type";
		Connection connection = null;
		PreparedStatement statement = null;
		ResultSet resultset = null;
		List<Map<String,Object>> list = new ArrayList<Map<String,Object>>();
		try{
			connection = builder.getConnection();
			logger.debug(sql);
			statement = connection.prepareStatement(sql);
			resultset = statement.executeQuery();
			while(resultset.next()){
				Map<String,Object> map = new HashMap<String,Object>();
				String label = builder.trackValue(resultset.getObject("Info_Name"));
				String data = builder.trackValue(resultset.getObject("Info_Type"));
				map.put("label", label);
				map.put("data", data);
				list.add(map);
			}
			builder.close(connection, statement, resultset);
		}catch(Exception e){
			builder.close(connection, statement, resultset);
			builder.trackException(e);
			this.setError(-900001, "参数初始化，获取角色id信息列表发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		return list;
	}
	
	public List<Map<String,Object>> getT_Info_Number_ObjectInfo(T_INFO_NUMBER_OBJECT sendObject){
		this.clear();
		String sql = "select Send_Group_ID,Send_Group_Name from T_Info_Number_Object where to_char(Insert_Time,'yyyy-mm-dd')>='"+sendObject.INSERT_TIME+"' and to_char(Insert_Time,'yyyy-mm-dd')<='"+sendObject.USER_ID+"' and Org_ID ='"+sendObject.ORG_ID+"'";
		if(sendObject.SEND_GROUP_NAME !="" && sendObject.SEND_GROUP_NAME.length() >0){
			sql += " and User_ID = '"+sendObject.SEND_GROUP_NAME+"'  order by Insert_Time desc";
		}else{
			sql += " order by Insert_Time desc";
		}
		System.out.println(sql);
		Connection connection = null;
		PreparedStatement statement = null;
		ResultSet resultset = null;
		List<Map<String,Object>> list = new ArrayList<Map<String,Object>>();
		try{
			connection = builder.getConnection();
			logger.debug(sql);
			statement = connection.prepareStatement(sql);
			resultset = statement.executeQuery();
			while(resultset.next()){
				Map<String,Object> map = new HashMap<String,Object>();
				String label = builder.trackValue(resultset.getObject("Send_Group_Name"));
				String data = builder.trackValue(resultset.getObject("Send_Group_ID"));
				map.put("label", label);
				map.put("data", data);
				list.add(map);
			}
			builder.close(connection, statement, resultset);
		}catch(Exception e){
			builder.close(connection, statement, resultset);
			builder.trackException(e);
			this.setError(-900001, "参数初始化，获取角色id信息列表发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		return list;
	}
	
	public List queryT_Info_Number_Object(String sendObject){
		this.clear();
		String sql = "select to_char(Amount) Amount,Language from T_Info_Number_Object";
		if(sendObject !="" && sendObject.length() >0){
			sql += " where Send_Group_ID = '"+sendObject+"'";
		}
		Connection connection = null;
		PreparedStatement statement = null;
		ResultSet rs = null;
		List list = new ArrayList();
		try{
			connection = builder.getConnection();
			logger.debug(sql);
			statement = connection.prepareStatement(sql);
			rs = statement.executeQuery();
			while(rs.next()){
				T_INFO_NUMBER_OBJECT  SendObject = new T_INFO_NUMBER_OBJECT();
				SendObject.AMOUNT = builder.trackValue(rs.getString("Amount"));
				SendObject.LANGUAGE = builder.trackValue(rs.getString("Language"));
				list.add(SendObject);
			}
			builder.close(connection, statement, rs);
		}catch(Exception e){
			builder.close(connection, statement, rs);
			builder.trackException(e);
			this.setError(-900001, "参数初始化，获取角色id信息列表发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		return list;
	}
	public List<Map<String,Object>> getT_Info_TypeAll(String type){
		this.clear();
		String sql = "select Info_Type,Info_Name from T_Info_Type";
		Connection connection = null;
		PreparedStatement statement = null;
		ResultSet resultset = null;
		List<Map<String,Object>> list = new ArrayList<Map<String,Object>>();
		try{
			connection = builder.getConnection();
			logger.debug(sql);
			statement = connection.prepareStatement(sql);
			resultset = statement.executeQuery();
			Map<String,Object> map1 = new HashMap<String,Object>();
			map1.put("label", "全部");
			map1.put("data", "");
			list.add(map1);
			while(resultset.next()){
				Map<String,Object> map = new HashMap<String,Object>();
				String label = builder.trackValue(resultset.getObject("Info_Name"));
				String data = builder.trackValue(resultset.getObject("Info_Type"));
				map.put("label", label);
				map.put("data", data);
				list.add(map);
			}
			builder.close(connection, statement, resultset);
		}catch(Exception e){
			builder.close(connection, statement, resultset);
			builder.trackException(e);
			this.setError(-900001, "参数初始化，获取角色id信息列表发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		return list;
	}
	public List<Map<String,Object>> getT_OrgID__OrgNameAll(String type){
		this.clear();
		String sql = "select ORG_ID,ORG_NAME from T_ORG_INFO where Status='0'";
		if(type!=null&&type.length()>0){
			sql += " and Org_Type='" + type+"'";
		}
		Connection connection = null;
		PreparedStatement statement = null;
		ResultSet resultset = null;
		List<Map<String,Object>> list = new ArrayList<Map<String,Object>>();
		try{
			connection = builder.getConnection();
			logger.debug(sql);
			statement = connection.prepareStatement(sql);
			resultset = statement.executeQuery();
			Map<String,Object> map1 = new HashMap<String,Object>();
			map1.put("label", "全部");
			map1.put("data", "");
			list.add(map1);
			while(resultset.next()){
				Map<String,Object> map = new HashMap<String,Object>();
				String label = resultset.getString("ORG_NAME").toString();
				String data = resultset.getString("ORG_ID").toString();
				map.put("label", label);
				map.put("data", data);
				list.add(map);
			}
			builder.close(connection, statement, resultset);
		}catch(Exception e){
			builder.close(connection, statement, resultset);
			builder.trackException(e);
			this.setError(-900001, "参数初始化，获取终端厂家信息列表发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		return list;
	}
	
	public List<Map<String,Object>> get_T_Org_Info_NoStatus(String type){
		this.clear();
		String sql = "select distinct Org_ID,Org_Name from T_Org_Info where Org_Type='3'";
		Connection connection = null;
		PreparedStatement statement = null;
		ResultSet resultset = null;
		List<Map<String,Object>> list = new ArrayList<Map<String,Object>>();
		try{
			connection = builder.getConnection();
			logger.debug(sql);
			statement = connection.prepareStatement(sql);
			resultset = statement.executeQuery();
			Map<String,Object> map1 = new HashMap<String,Object>();
			map1.put("label", "全部");
			map1.put("data", "");
			list.add(map1);
			while(resultset.next()){
				Map<String,Object> map = new HashMap<String,Object>();
				String label = builder.trackValue(resultset.getObject("Org_Name"));
				String data = builder.trackValue(resultset.getObject("Org_ID"));
				map.put("label", label);
				map.put("data", data);
				list.add(map);
			}
			builder.close(connection, statement, resultset);
		}catch(Exception e){
			builder.close(connection, statement, resultset);
			builder.trackException(e);
			this.setError(-900001, "参数初始化，获取信息列表发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		return list;
	}
	
	public List<Map<String,Object>> getT_Org_Info_Org_ID_Org_Name_NoStatus(String type){
		this.clear();
		String sql = "select distinct Org_ID,Org_Name from T_Org_Info where Org_Type = '2'";
		Connection connection = null;
		PreparedStatement statement = null;
		ResultSet resultset = null;
		List<Map<String,Object>> list = new ArrayList<Map<String,Object>>();
		try{
			connection = builder.getConnection();
			logger.debug(sql);
			statement = connection.prepareStatement(sql);
			resultset = statement.executeQuery();
			while(resultset.next()){
				Map<String,Object> map = new HashMap<String,Object>();
				String label = builder.trackValue(resultset.getObject("Org_Name"));
				String data = builder.trackValue(resultset.getObject("Org_ID"));
				map.put("label", label);
				map.put("data", data);
				list.add(map);
			}
			builder.close(connection, statement, resultset);
		}catch(Exception e){
			builder.close(connection, statement, resultset);
			builder.trackException(e);
			this.setError(-900001, "参数初始化，获取角色id信息列表发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		return list;
	}
	
	public List<Map<String,Object>> getT_User_InfoAllWhere(String type){
		this.clear();
		String sql = "select User_ID,User_Name from T_User_Info where Org_ID='"+type+"'";
		Connection connection = null;
		PreparedStatement statement = null;
		ResultSet resultset = null;
		List<Map<String,Object>> list = new ArrayList<Map<String,Object>>();
		try{
			connection = builder.getConnection();
			logger.debug(sql);
			statement = connection.prepareStatement(sql);
			resultset = statement.executeQuery();
			while(resultset.next()){
				Map<String,Object> map = new HashMap<String,Object>();
				String label = resultset.getString("User_Name").toString();
				String data = resultset.getString("User_ID").toString();
				map.put("label", label);
				map.put("data", data);
				list.add(map);
			}
			builder.close(connection, statement, resultset);
		}catch(Exception e){
			builder.close(connection, statement, resultset);
			builder.trackException(e);
			this.setError(-900001, "参数初始化，获取用户信息列表发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		return list;
	}
	
	public List<Map<String,Object>> get_T_Org_Info_NoStatusWhere(String type){
		this.clear();
		String sql = "select distinct Org_ID,Org_Name from T_Org_Info where Org_ID='"+type+"'";
		Connection connection = null;
		PreparedStatement statement = null;
		ResultSet resultset = null;
		List<Map<String,Object>> list = new ArrayList<Map<String,Object>>();
		try{
			connection = builder.getConnection();
			logger.debug(sql);
			statement = connection.prepareStatement(sql);
			resultset = statement.executeQuery();
			while(resultset.next()){
				Map<String,Object> map = new HashMap<String,Object>();
				String label = builder.trackValue(resultset.getObject("Org_Name"));
				String data = builder.trackValue(resultset.getObject("Org_ID"));
				map.put("label", label);
				map.put("data", data);
				list.add(map);
			}
			builder.close(connection, statement, resultset);
		}catch(Exception e){
			builder.close(connection, statement, resultset);
			builder.trackException(e);
			this.setError(-900001, "参数初始化，获取信息列表发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		return list;
	}
	
	public List<Map<String,Object>> getT_OrgID__OrgNames(String[] type){
		this.clear();
		String sql = "select ORG_ID,ORG_NAME from T_ORG_INFO where  Status='0'";
		if(type.length>0){
			String tempSQL="";
			
			for(int i=0;i<type.length;i++){
				tempSQL +=" Org_Type='" + type[i]+"'";
				if(i<type.length-1){
					tempSQL+=" or";
				}
			}
			sql+="  and (" + tempSQL + ")";
		}
		Connection connection = null;
		PreparedStatement statement = null;
		ResultSet resultset = null;
		List<Map<String,Object>> list = new ArrayList<Map<String,Object>>();
		try{
			connection = builder.getConnection();
			logger.debug(sql);
			statement = connection.prepareStatement(sql);
			resultset = statement.executeQuery();
			while(resultset.next()){
				Map<String,Object> map = new HashMap<String,Object>();
				String label = resultset.getString("ORG_NAME").toString();
				String data = resultset.getString("ORG_ID").toString();
				map.put("label", label);
				map.put("data", data);
				list.add(map);
			}
			builder.close(connection, statement, resultset);
		}catch(Exception e){
			builder.close(connection, statement, resultset);
			builder.trackException(e);
			this.setError(-900001, "参数初始化，获取终端厂家信息列表发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		return list;
	}

}
