package cn.ucans.flex.services;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import chobits.db.DBBuilder;
import chobits.init.ChobitsConfig;
import chobits.log.Logger;
import cn.ucans.adapter.OrgAdapter;
import cn.ucans.adapter.RoleAdapter;
import cn.ucans.bean.T_ORG_INFO;
import cn.ucans.bean.T_USER_INFO;
import cn.ucans.bean.T_ROLE_INFO;
import cn.ucans.flex.FlexReponse;
import cn.ucans.flex.FlexRequest;
import cn.ucans.param.ParamHandler;

public class UserInfoRoleFlexService extends FlexService{
	
	private Logger logger = new Logger(UserInfoRoleFlexService.class.getName());
	
	public FlexReponse doTransaction(FlexRequest request){
		logger.debug("-----------------------------权限信息管理任务 启动-------------------------------");
		FlexReponse reponse = null;
		
		try{			
			super.checkRole(request);
			int ope_id = request.ope_id;	
			switch(ope_id){
				case 1010:
					reponse = this.addHuman(request);
					break;
				case 1011:
					reponse = this.updateHuman(request);
					break;
				case 1012:
					reponse = this.deleteHuman(request);
					break;
				case 1013:
					reponse = this.queryHuman(request);
					break;
				case 1014:
					reponse = this.addRole(request);
					break;
				case 1015:
					reponse = this.updateRole(request);
					break;
				case 1016:
					reponse = this.deleteRole(request);
					break;
				case 1017:
					reponse = this.queryRole(request);
					break;
				case 1018:
					reponse = this.initRoleTree(request);
					break;
				case 1019:
					reponse = this.initUpdateRoleTree(request);
					break;
				case 1020:
					reponse = this.checkLoginUserInfo(request);
					break;
				case 1021:
					reponse = this.changeUserPassword(request);
					break;
				case 1022:
					reponse = this.exitLogin(request);
					break;
				case 1023:
					reponse = this.forceChangePassword(request);
					break;
				case 1024:
					reponse = this.initChangeMenuRoleTree(request);
					break;
				case 1025:
					reponse = this.updateMenuNameChangeRole(request);
					break;
				case 1026:
					reponse = this.queryUserLoginHis(request);
					break;
				case 1027:
					reponse = this.intQueryLoginHisParm(request);
					break;
				default:
					break;
			}
			logger.debug("-----------------------------权限信息管理任务 成功完成-------------------------------");
		}catch(Exception e){
			logger.error(e.getMessage());
			reponse = new FlexReponse(request,-1,e.getMessage());	
			logger.debug("-----------------------------权限信息管理任务 执行失败-------------------------------");
		}
		
		return reponse;
	}
	
	private FlexReponse addRole(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();
		T_ROLE_INFO role = (T_ROLE_INFO)request.getBean("role");
		String xml_format = (String)request.getBean("xml");
		
		int count = builder.getCount("T_Role_Info", "Role_ID='"+role.ROLE_ID+"'");
		if(builder.SQL_Error_Code != 0){
			throw new Exception("权限管理，校验角色编号发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}else{
			if(count>0){
				throw new Exception("权限管理，角色编号（"+role.ROLE_ID+"）已经存在！");
			}
		}
		List<String> list = new ArrayList<String>();
		RoleAdapter ra = new RoleAdapter(xml_format);
		list = ra.getSetupSQLList(role.ROLE_ID) ;
		if(list != null){
			String sql = "insert into T_Role_Info (Role_ID,Role_Name,Role_Notes,Create_User_ID,Create_Time) values ";
			sql += "('"+role.ROLE_ID+"','"+role.ROLE_NAME+"','"+role.ROLE_NOTES+"','"+request.user_id+"',sysdate)";
			list.add(sql);			
		}else{
			list = new ArrayList<String>();
			String sql = "insert into T_Role_Info (Role_ID,Role_Name,Role_Notes,Create_User_ID,Create_Time) values ";
			sql += "('"+role.ROLE_ID+"','"+role.ROLE_NAME+"','"+role.ROLE_NOTES+"','"+request.user_id+"',sysdate)";
			list.add(sql);	
		}
		
//		for(int m=0;m<list.size();m++){
//			logger.debug("批处理sql-> "+list.get(m));
//		}
		builder.excute(list);
		if(builder.SQL_Error_Code != 0){
			throw new Exception("权限管理，添加角色信息发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,"添加角色("+role.ROLE_ID+"，"+role.ROLE_NAME+")信息成功！");
		return reponse;
	}
	
	private FlexReponse updateRole(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();
		T_ROLE_INFO role = (T_ROLE_INFO)request.getBean("role");
		String xml_format = (String)request.getBean("xml");
		
		int count = builder.getCount("T_Role_Info", "Role_ID='"+role.ROLE_ID+"'");
		if(builder.SQL_Error_Code != 0){
			throw new Exception("权限管理，校验角色编号发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}else{
			if(count==0){
				throw new Exception("权限管理，角色编号（"+role.ROLE_ID+"）不存在！");
			}
		}
		
		List<String> list = new ArrayList<String>();
		if(!role.ROLE_ID.equals("00A")){
			RoleAdapter ra = new RoleAdapter(xml_format);
			list = ra.getSetupSQLList(role.ROLE_ID) ;
			if(list == null){
				list = new ArrayList<String>();
			}
			String sql_delete = "delete from T_Role_Menu where Role_ID='"+role.ROLE_ID+"'";
			list.add(0,sql_delete);
		}
		
		String sql_update = "update T_Role_Info set ";
		sql_update += "Role_Name = '"+role.ROLE_NAME+"',";
		sql_update += "Role_Notes = '"+role.ROLE_NOTES+"'";
		sql_update += " where Role_ID='"+role.ROLE_ID+"'";
		
		list.add(sql_update);
		
		builder.excute(list);
		if(builder.SQL_Error_Code != 0){
			throw new Exception("权限管理，更新该角色旧的存储信息发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,"更新角色("+role.ROLE_ID+"，"+role.ROLE_NAME+")信息成功！");
		return reponse;
	}
	
	private FlexReponse updateMenuNameChangeRole(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();
		T_ROLE_INFO role = (T_ROLE_INFO)request.getBean("role");
		String xml_format = (String)request.getBean("xml");
		
		int count = builder.getCount("T_Role_Info", "Role_ID='"+role.ROLE_ID+"'");
		if(builder.SQL_Error_Code != 0){
			throw new Exception("权限管理，校验角色编号发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}else{
			if(count==0){
				throw new Exception("权限管理，角色编号（"+role.ROLE_ID+"）不存在！");
			}
		}
		
		List<String> list = new ArrayList<String>();
		if(!role.ROLE_ID.equals("00A")){
			RoleAdapter ra = new RoleAdapter(xml_format);
			list = ra.getMenuChangeSQLList(role.ROLE_ID) ;
			if(list == null){
				list = new ArrayList<String>();
			}
		}else{
			throw new Exception("系统保护，禁止改变“00A”角色所属的功能菜单！");
		}
		
		builder.excute(list);
		if(builder.SQL_Error_Code != 0){
			throw new Exception("权限管理，更新该角色旧的存储信息发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,"更新角色("+role.ROLE_ID+")功能菜单信息成功！");
		return reponse;
	}
	
	private FlexReponse deleteRole(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();
		T_ROLE_INFO role = (T_ROLE_INFO)request.getBean("role");
		
		int count = builder.getCount("T_User_Info", "Role_ID='"+role.ROLE_ID+"'");
		if(builder.SQL_Error_Code != 0){
			throw new Exception("权限管理，校验用户表中的角色编号时发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}else{
			if(count>0){
				throw new Exception("权限管理，已经有"+count+"个用户使用了"+role.ROLE_ID+"角色，请先处理这些人员，再进行此操作！");
			}
		}
		
		List<String> list = new ArrayList<String>();
		String sql_delete_info = "delete from T_Role_Info where Role_ID='"+role.ROLE_ID+"'";
		String sql_delete_menu = "delete from T_Role_Menu where Role_ID='"+role.ROLE_ID+"'";
		list.add(sql_delete_info);
		list.add(sql_delete_menu);
		builder.excute(list);
		if(builder.SQL_Error_Code != 0){
			throw new Exception("权限管理，删除角色旧的存储信息发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,"删除角色("+role.ROLE_ID+"，"+role.ROLE_NAME+")信息成功！");
		return reponse;
	}
	
	private FlexReponse queryRole(FlexRequest request) throws Exception{
		T_ROLE_INFO role = (T_ROLE_INFO) request.getBean("role");
		DBBuilder builder = new DBBuilder();
		String sql = "select Role_ID,Role_Name,Role_Notes,Create_User_ID,Create_Time from T_Role_Info";
		boolean haswhere = false;
		if(role.ROLE_ID != null && role.ROLE_ID.trim().length()>0){
			sql += " where Role_ID='"+role.ROLE_ID+"'";
			haswhere = true;
		}
		if(role.ROLE_NAME != null && role.ROLE_NAME.trim().length()>0){
			if(haswhere){
				sql += " and Role_Name='"+role.ROLE_NAME+"'";
			}else{
				sql += " where Role_Name='"+role.ROLE_NAME+"'";
				haswhere = true;
			}
		}
		
		List<Map<String,Object>> list = builder.queryList(sql);
		if(builder.SQL_Error_Code != 0){
			throw new Exception("权限管理，查询角色信息发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list", list);
		return reponse;
	}
	
	private FlexReponse initRoleTree(FlexRequest request) throws Exception{		
		RoleAdapter ra = new RoleAdapter();
		
		String xml_init_role = ra.getInitRoleXMLString("-1", 0, "00A");
		if(xml_init_role == null){
			throw new Exception("初始化权限菜单树失败，原因："+ra.Error_Message);
		}else{
			logger.debug("初始化权限树: "+xml_init_role);
		}
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("xml", xml_init_role);		
		return reponse;
	}
	
	private FlexReponse initUpdateRoleTree(FlexRequest request) throws Exception{	
		T_ROLE_INFO role = (T_ROLE_INFO)request.getBean("role");
		RoleAdapter ra = new RoleAdapter();		
		String xml_init_role = ra.getInitUpdateRoleXMLString(role.ROLE_ID);
		if(xml_init_role == null){
			throw new Exception("初始化权限菜单树失败，原因："+ra.Error_Message);
		}else{
			logger.debug("初始化更新所用的权限树: "+xml_init_role);
		}
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("xml", xml_init_role);		
		return reponse;
	}
	
	private FlexReponse initChangeMenuRoleTree(FlexRequest request) throws Exception{	
		T_ROLE_INFO role = (T_ROLE_INFO)request.getBean("role");
		RoleAdapter ra = new RoleAdapter();		
		String xml_init_role = ra.getInitchangeMenuRoleXMLString(role.ROLE_ID);
		if(xml_init_role == null){
			throw new Exception("初始化权限菜单树失败，原因："+ra.Error_Message);
		}else{
			logger.debug("初始化更新所用的权限树: "+xml_init_role);
		}
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("xml", xml_init_role);		
		return reponse;
	}
	
	private FlexReponse addHuman(FlexRequest request) throws Exception{		
		DBBuilder builder = new DBBuilder();
		T_USER_INFO user = (T_USER_INFO) request.getBean("user");
		
		int count = builder.getCount("T_User_Info", "User_ID='"+user.USER_ID+"'");
		if(builder.SQL_Error_Code != 0){
			throw new Exception("权限管理，校验用户编号发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}else{
			if(count>0){
				throw new Exception("权限管理，用户编号（"+user.USER_ID+"）已经存在！");
			}
		}
		
		String sql = "insert into T_User_Info (User_ID,User_Name,Password,Role_ID,Org_ID,User_Area_Code,Status,Create_Time) values ";
		sql += "('"+user.USER_ID+"','"+user.USER_NAME+"','"+user.PASSWORD+"','"+user.ROLE_ID+"','"+user.ORG_ID+"','"+user.USER_AREA_CODE+"','0',sysdate)";
		builder.excute(sql);
		if(builder.SQL_Error_Code != 0){
			throw new Exception("添加人员信息发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,"添加人员("+user.USER_ID+"，"+user.USER_NAME+")成功！");
		return reponse;
	}
	
	private FlexReponse updateHuman(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();
		T_USER_INFO user = (T_USER_INFO) request.getBean("user");
		
		int count_role_id = builder.getCount("T_Role_Info", "Role_ID='"+user.ROLE_ID+"'");
		if(builder.SQL_Error_Code != 0){
			throw new Exception("权限管理，校验角色编号发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}else{
			if(count_role_id==0){
				throw new Exception("权限管理，角色编号（"+user.ROLE_ID+"）不存在！");
			}
		}
		
		int count_area_code = builder.getCount("T_AREA_CODE_PARM", "AREA_CODE='"+user.USER_AREA_CODE+"'");
		if(builder.SQL_Error_Code != 0){
			throw new Exception("权限管理，校验角地区代码发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}else{
			if(count_area_code==0){
				throw new Exception("权限管理，地区代码（"+user.USER_AREA_CODE+"）不存在！");
			}
		}
		
		String sql = "update T_USER_INFO set ";		
		sql += " User_Name = '"+user.USER_NAME+"',";//
		//sql += " Password = '"+user.PASSWORD+"',";//
		sql += " Role_ID = '"+user.ROLE_ID+"',";//
		sql += " User_Area_Code = '"+user.USER_AREA_CODE+"'";//
		sql += " where User_ID = '"+user.USER_ID+"'";
		builder.excute(sql);			
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("更新人员信息发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,"更新人员("+user.USER_ID+"，"+user.USER_NAME+")信息成功！");
		return reponse;
	}

	private FlexReponse deleteHuman(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();
		T_USER_INFO user = (T_USER_INFO) request.getBean("user");
		
		String sql_delete_human = "update T_USER_INFO set Status='1' where User_ID = '"+user.USER_NAME+"'";
		builder.excute(sql_delete_human);		
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("删除人员信息发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,"删除人员("+user.USER_ID+"，"+user.USER_NAME+")信息成功！");
		return reponse;
	}
	
	private FlexReponse queryHuman(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();	
		T_USER_INFO user = (T_USER_INFO) request.getBean("user");
		
		String sql = " select T_User_Info.Org_ID as Org_ID,T_User_Info.User_ID as User_ID,T_User_Info.Password as Password,T_User_Info.User_Name as User_Name,T_User_Info.Role_ID as Role_ID,T_Role_Info.Role_Name as Role_Name,T_Org_Info.Org_Name as Org_Name,T_User_Info.User_Area_Code as User_Area_Code,T_AREA_CODE_PARM.Area_Name as Area_Name,T_User_Info.Status as Status,T_User_Info.Create_Time as Create_Time,T_User_Info.Last_Login_Time as Last_Login_Time";
		sql += " from T_User_Info,T_Role_Info,T_Org_Info,T_AREA_CODE_PARM";
		sql += " where T_User_Info.Org_ID='"+user.ORG_ID+"' and T_User_Info.Org_ID=T_Org_Info.Org_ID and T_User_Info.Role_ID=T_Role_Info.Role_ID and T_User_Info.User_Area_Code=T_AREA_CODE_PARM.Area_Code";
		if(user.STATUS != null && user.STATUS.trim().length()>0){
			sql += " and T_User_Info.Status = '"+user.STATUS+"' ";
		}
		if(user.USER_ID != null && user.USER_ID.trim().length()>0){
			sql += " and T_User_Info.USER_ID = '"+user.USER_ID+"' ";
		}
		if(user.USER_NAME != null && user.USER_NAME.trim().length()>0){
			sql += " and T_User_Info.USER_NAME = '"+user.USER_NAME+"' ";
		}
		List<Map<String,Object>> list = builder.queryList(sql);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("查询人员信息发生错误："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}

		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list", list);
		return reponse;
	}
	
	private FlexReponse checkLoginUserInfo(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();	
		T_USER_INFO user = (T_USER_INFO) request.getBean("user");
		String type = (String) request.getBean("type");
		int count_user = builder.getCount("T_User_Info", "User_ID='"+user.USER_ID+"' and Password='"+user.PASSWORD+"'");
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("用户登陆，验证用户编号和用户密码出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}else{
			if(count_user==0){
				throw new Exception("用户登陆，用户编号或用户密码错误！");
			}
		}
		String sql_role = "select T_User_Info.User_ID as User_ID,T_User_Info.User_Name as User_Name,T_Role_Info.Role_ID as Role_ID,T_Role_Info.Role_Name as Role_Name,T_Org_Info.Org_ID as Org_ID,T_Org_Info.Org_Name as Org_Name,T_Org_Info.Status as Status ";
		sql_role += " from T_Role_Info,T_User_Info,T_Org_Info ";
		sql_role += " where T_User_Info.User_ID='"+user.USER_ID+"' and T_User_Info.Password='"+user.PASSWORD+"' and T_User_Info.ORG_ID = T_Org_Info.ORG_ID and T_User_Info.Role_ID=T_Role_Info.Role_ID";
		Map<String,Object> map = builder.queryMap(sql_role);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("用户登陆，获取该用户（"+user.ROLE_ID+"）权限标识出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		String org_id = (String) map.get("ORG_ID");
		String org_name = (String) map.get("ORG_NAME");
		String role_id = (String) map.get("ROLE_ID");
		String user_name = (String) map.get("USER_NAME");
		String role_name = (String) map.get("ROLE_NAME");
		String status = (String) map.get("STATUS");
		if(!status.equals("0")){
			throw new Exception("用户登陆，您所管辖的组织（"+org_id+","+org_name+"）已经被注销，因此无发登陆！");
		}
		RoleAdapter ra = new RoleAdapter();
		String Role_XML = ra.getInitchangeMenuRoleXMLString(role_id);
		logger.debug("用户权限树："+Role_XML);
		if(Role_XML == null){
			throw new Exception("用户登陆，您所在角色没有被授予任何模块的管辖权限，无法使用本系统！");
		}
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.org_id = org_id;
		reponse.user_id = user.USER_ID;
		reponse.set("Role_XML", Role_XML);
		
		long last_login_sql = builder.getOracleSequreNextValue("Login_SEQ");
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("用户登陆，获取用户登陆序列号出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}else{
			reponse.set("LoginSEQ", String.valueOf(last_login_sql));
		}
		
		List<String> list_sql = new ArrayList<String>();
		String update_sql = "update T_User_Info set Last_Login_Time=sysdate where User_ID='"+user.USER_ID+"'";
		String insert_sql = "insert into T_User_Login_His (Login_ID,User_ID,User_Name,Org_ID,Role_ID,Login_Time) values ";
		insert_sql += "("+last_login_sql+",'"+user.USER_ID+"','"+user_name+"','"+org_id+"','"+role_id+"',sysdate)";
		list_sql.add(update_sql);
		list_sql.add(insert_sql);
		builder.excute(list_sql);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("用户登陆，更新用户登录日志出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		reponse.set("strPubURL", ChobitsConfig.VisitURL);
		reponse.set("LoginRole_ID", role_id);
		return reponse;
	}
	
	private FlexReponse changeUserPassword(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();	
		T_USER_INFO user = (T_USER_INFO) request.getBean("user");
		String password_old = (String) request.getBean("password_old");
		int count_user = builder.getCount("T_User_Info", "User_ID='"+user.USER_ID+"' and Password='"+password_old+"'");
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("用户管理，验证用户编号和用户密码出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}else{
			if(count_user==0){
				throw new Exception("用户管理，修改密码出错，原因：原密码输入错误！");
			}
		}
		
		String update_sql = "update T_User_Info set Password='"+user.PASSWORD+"' where User_ID='"+user.USER_ID+"'";
		builder.excute(update_sql);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("用户管理，修改用户密码出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,"密码修改成功！");
		return reponse;
	}
	
	private FlexReponse exitLogin(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();	
		String user_id = request.user_id;
		String login_seq = (String) request.getBean("Login_SEQ");
		
		String update_sql = "update T_User_Login_His set Logout_Time=sysdate where Login_ID="+login_seq+" and User_ID='"+user_id+"'";
		builder.excute(update_sql);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("用户管理，修改用户密码出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,"登陆成功退出！");
		return reponse;
	}
	
	private FlexReponse forceChangePassword(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();	
		T_USER_INFO user = (T_USER_INFO) request.getBean("user");
		
		int count_user = builder.getCount("T_User_Info", "User_ID='"+user.USER_ID+"'");
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("用户管理，验证用户编号和用户密码出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}else{
			if(count_user==0){
				throw new Exception("用户管理，强制修改密码出错，原因：该用户("+user.USER_ID+")已经不存在！");
			}
		}
		
		String update_sql = "update T_User_Info set Password='"+user.PASSWORD+"' where User_ID='"+user.USER_ID+"'";
		builder.excute(update_sql);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("用户管理，修改用户密码出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,"密码修改成功！");
		return reponse;
	}
	
	private FlexReponse queryUserLoginHis(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();	
		T_USER_INFO user = (T_USER_INFO) request.getBean("user");
		T_ORG_INFO org = (T_ORG_INFO) request.getBean("org");
		String notbefore = (String) request.getBean("notbefore");
		String notafter = (String) request.getBean("notafter");		
		
		String sql = " select T_User_Login_His.User_ID as User_ID,T_User_Login_His.User_Name as User_Name,T_User_Login_His.Login_Time as Login_Time,T_Org_Info.Org_Name as Org_Name,T_Role_Info.Role_Name as Role_Name ";
		sql += " from T_User_Login_His,T_Org_Info,T_Role_Info";
		sql += " where T_User_Login_His.Login_Time >= to_date('"+notbefore+"','yyyy-mm-dd hh24:mi:ss') and T_User_Login_His.Login_Time <= to_date('"+notafter+"','yyyy-mm-dd hh24:mi:ss') and T_User_Login_His.Org_ID=T_Org_Info.Org_ID and T_User_Login_His.Role_ID=T_Role_Info.Role_ID ";
		if(user.USER_ID != null && user.USER_ID.trim().length()>0){
			sql += " and T_User_Login_His.User_ID = '"+user.USER_ID+"' ";
		}
		if(user.USER_NAME != null && user.USER_NAME.trim().length()>0){
			sql += " and T_User_Login_His.User_Name = '"+user.USER_NAME+"' ";
		}
		if(user.ROLE_ID != null && user.ROLE_ID.trim().length()>0){
			sql += " and T_User_Login_His.Role_ID = '"+user.ROLE_ID+"' ";
		}
		if(org.ORG_ID != null && org.ORG_ID.trim().length()>0){
			sql += " and T_Org_Info.Org_ID = '"+org.ORG_ID+"' ";
		}
		if(org.ORG_TYPE != null && org.ORG_TYPE.trim().length()>0){
			sql += " and T_Org_Info.Org_Type = '"+org.ORG_TYPE+"' ";
		}
		sql += " order by T_User_Login_His.Login_Time";
		List<Map<String,Object>> list = builder.queryList(sql);
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("用户登录信息查询，查询出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list", list);
		return reponse;
	}
	
	private FlexReponse intQueryLoginHisParm(FlexRequest request) throws Exception{
		ParamHandler handler = new ParamHandler();
		Map<String,Object> map = new HashMap<String,Object>();
		map.put("label", "全部");
		map.put("data", "");
		List<Map<String,Object>> list_role_id = handler.getT_Role_Info__Role_ID(null);
		list_role_id.add(0, map);					//0平台,1终端厂家,2代销商,3酒店,4广告业主
		List<Map<String,Object>> list_org_id = handler.getT_Org_Info_Org_ID_Org_Name("('0','2','3','4')", "('0','1')");
		list_org_id.add(0, map);
		List<Map<String,Object>> list_user_id = handler.getT_User_Info_User_ID();
		list_user_id.add(0, map);
		
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list_role_id", list_role_id);
		reponse.set("list_org_id", list_org_id);
		reponse.set("list_user_id", list_user_id);
		return reponse;
	}
}
