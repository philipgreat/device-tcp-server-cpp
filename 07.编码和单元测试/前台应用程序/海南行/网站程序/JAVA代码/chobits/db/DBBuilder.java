package chobits.db;

import java.io.File;
import java.io.IOException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;

import javax.naming.Context;
import javax.naming.InitialContext;

import jxl.Workbook;
import jxl.write.Label;
import jxl.write.WritableSheet;
import jxl.write.WritableWorkbook;
import jxl.write.WriteException;

import org.jdom.Element;

import chobits.common.FileUtil;
import chobits.common.PageUtil;
import chobits.init.ChobitsConfig;
import chobits.log.Logger;
import chobits.xml.XMLBuilder;

public class DBBuilder {
	
	public static String ROW_COUNT = "ROW_COUNT";
	public static String PAGE_COUNT = "PAGE_COUNT";
	public static String CURRENT_PAGE_NO = "CURRENT_PAGE_NO";
	public static String DATA_LIST = "DATA_LIST";
	
	private static String DataSouceName = "";
	private static String LOOKUPPATH = null;
	private static String Type = "1";
	
	private static String Driver = null;
	private static String URI = null;
	private static String User = null;
	private static String Password = null;
	
	public String SQL_Error_Message = null;
	public int SQL_Error_Code = 0;
	
	private Logger logger = new Logger(DBBuilder.class.getName());
	
	public DBBuilder(){
		
	}
	
	public static void initCommonType(){
		try{
			DBBuilder.Driver = "oracle.jdbc.driver.OracleDriver";
			DBBuilder.URI = "jdbc:oracle:thin:@192.168.1.6:1521:carsmart";
			DBBuilder.User = "happy";
			DBBuilder.Password = "happy";
			DBBuilder.Type = "2";
			Class.forName(DBBuilder.Driver);
		}catch(Exception e){
			e.printStackTrace();
		}
//		try{
//			DBBuilder.Driver = "oracle.jdbc.driver.OracleDriver";
//			DBBuilder.URI = "jdbc:oracle:thin:@192.168.1.6:1521:ORCL";
//			DBBuilder.User = "happy";
//			DBBuilder.Password = "happy";
//			DBBuilder.Type = "2";
//			Class.forName(DBBuilder.Driver);
//		}catch(Exception e){
//			e.printStackTrace();
//		}
	}
	
	public static void init(Element root_element){
		try{
			DBBuilder.Type = root_element.getAttributeValue("type").trim();
			if(DBBuilder.Type.equals("1")){
				Element element_souce = root_element.getChild("Souce");
				DBBuilder.LOOKUPPATH = element_souce.getChildTextTrim("LookUP");
				DBBuilder.DataSouceName = element_souce.getChildTextTrim("Name");
			}else if(DBBuilder.Type.equals("2")){
				Element element_single = root_element.getChild("Single");
				DBBuilder.Driver = element_single.getChildTextTrim("Driver");
				DBBuilder.URI = element_single.getChildTextTrim("URI");
				DBBuilder.User = element_single.getChildTextTrim("User");
				DBBuilder.Password = element_single.getChildTextTrim("Password");
				Class.forName(DBBuilder.Driver);
			}
		}catch(Exception e){
			e.printStackTrace();
		}
	}
	
	public Connection getConnection(){
		Connection connection = null;
		try{
			if(DBBuilder.Type.equals("1")){
				Context initCtx = new InitialContext();
		        Context ctx = (Context) initCtx.lookup(DBBuilder.LOOKUPPATH);
		        Object obj = (Object) ctx.lookup(DBBuilder.DataSouceName);
		        javax.sql.DataSource ds = (javax.sql.DataSource)obj;
		        connection = ds.getConnection();
		        logger.debug("connect db pool "+DBBuilder.LOOKUPPATH+":"+DBBuilder.DataSouceName);
		        logger.debug("connect context "+connection.toString());
			}else if(DBBuilder.Type.equals("2")){
				connection = DriverManager.getConnection(DBBuilder.URI, DBBuilder.User, DBBuilder.Password);
			}
			
		}catch(Exception e){
			logger.error("connection adapter error:"+e.getMessage());
		}
        return connection;
	}
	
	public int excute(String sql){
		this.clearTempStatement();
		int excute_count = 0;
		Connection connection  = null;
		PreparedStatement statement = null;
		ResultSet resultset = null;
		boolean ok = false;
		try{
			connection = this.getConnection();
			if(connection == null){
				throw new Exception("db connection is null, please check db souce config currectly !");
			}
			logger.debug(sql);
			statement = connection.prepareStatement(sql);	
			excute_count = statement.executeUpdate();
			this.close(connection, statement, resultset);
			ok = true;
		}catch(Exception e){
			this.close(connection, statement, resultset);
			this.trackException(e);
		}
		return excute_count;
	}
	
	public int excute(List<String> list){
		this.clearTempStatement();
		int excute_count = 0;
		Connection connection  = null;
		PreparedStatement statement = null;
		ResultSet resultset = null;
		boolean ok = false;
		try{
			connection = this.getConnection();
			if(connection == null){
				throw new Exception("db connection is null, please check db souce config currectly !");
			}
			connection.setAutoCommit(false);
			int temp_count = 0;
			for(int i=0;i<list.size();i++){
				String sql = list.get(i);
				logger.debug(sql);
				statement = connection.prepareStatement(sql);	
				temp_count = statement.executeUpdate();
				statement.close();
				if(temp_count > 0){
					excute_count += temp_count;
				}
			}
			connection.commit();
			this.close(connection, statement, resultset);
			ok = true;
		}catch(Exception e){
			if(connection != null){
				try {
					connection.rollback();
					this.close(connection, statement, resultset);
				} catch (SQLException e1) {
					this.close(connection, statement, resultset);
				}
			}
			this.close(connection, statement, resultset);
			this.trackException(e);
		}
		return excute_count;
	}
	
	public long getOracleSequreNextValue(String seqName){
		this.clearTempStatement();
		Connection connection  = null;
		PreparedStatement statement = null;
		ResultSet resultset = null;
		String sql = "select "+seqName+".nextval as NEXTVALUE from dual";
		logger.debug(sql);
		long index = -1;
		try{
			connection = this.getConnection();
			if(connection == null){
				throw new Exception("db connection is null, please check db souce config currectly !");
			}
			statement = connection.prepareStatement(sql);
			resultset = statement.executeQuery();
			while(resultset.next()){
			    index = Long.parseLong(resultset.getObject("NEXTVALUE").toString());
			}			 
			this.close(connection, statement, resultset);
		}catch(Exception e){
			this.close(connection, statement, resultset);
			this.trackException(e);
		}		
		return index;
	}
	
	public Map<String, Object> queryPage(String filelds,String table,String solution,int pageNo){
		this.clearTempStatement();
		Connection connection = null;
		PreparedStatement statement = null;
		ResultSet resultset = null;
		Map<String, Object> return_map = null;
		PageUtil page = null;
		try{
			return_map = new HashMap<String, Object>();
			List<Map<String, Object>> list = new ArrayList<Map<String, Object>>();
			
			connection = this.getConnection();
			String count_sql = "";
			if(solution == null || solution.trim().length() == 0){
				count_sql = "select count(*) as TT_NO from "+table;
			}else{
				count_sql = "select count(*) as TT_NO from "+table + " where "+solution;
			}
			logger.debug(count_sql);
			if(connection == null){
				throw new Exception("db connection is null, please check db souce config currectly !");
			}
			statement = connection.prepareStatement(count_sql);
			resultset = statement.executeQuery();
			int count = 0;
			while(resultset.next()){
				count = resultset.getInt("TT_NO");
			}
			
			resultset.close();
			statement.close();
			
			if(count > 0){
				page = new PageUtil(count, pageNo);
				String query_sql = "";
				if(solution == null || solution.trim().length() == 0){
					query_sql = "select * from (select "+filelds+" from "+table+") where rownum<="+page.getEndRowNum()+" and rownum>="+page.getStartRowNum();
				}else{
					query_sql = "select y.* from (select x.*,rownum numbers from (select "+filelds+" from "+table+" where "+solution+") x where rownum<="+page.getEndRowNum()+") y where y.numbers>="+page.getStartRowNum();
				}
				logger.debug(query_sql);
				statement = connection.prepareStatement(query_sql);
				resultset = statement.executeQuery();
				ResultSetMetaData rsmd = statement.getMetaData();
				int columnCount = rsmd.getColumnCount();
				
				
				
				while(resultset.next()){
					Map<String, Object> map = new HashMap<String, Object>();
					for(int i=1;i<=columnCount;i++){
						String type = rsmd.getColumnTypeName(i);
						String name = rsmd.getColumnName(i);
						if(type.equals("DATE")){
							String value = this.trackValue(resultset.getDate(name))+" "+this.trackValue(resultset.getTime(name));
							if(value.equals(" ")){
								value = "";
							}
							map.put(name, value);							
						}else{
							map.put(name, this.trackValue(resultset.getObject(name)));
						}
					}
					list.add(map);
				}				
				return_map.put(DBBuilder.ROW_COUNT, count);
				return_map.put(DBBuilder.PAGE_COUNT, page.getPageCount());
				return_map.put(DBBuilder.CURRENT_PAGE_NO, page.getCurrentPageNO());
			}else{
				return_map.put(DBBuilder.ROW_COUNT, 0);
				return_map.put(DBBuilder.PAGE_COUNT, 0);
				return_map.put(DBBuilder.CURRENT_PAGE_NO, 0);
			}
			return_map.put(DBBuilder.DATA_LIST, list);
			this.close(connection, statement, resultset);
		}catch(Exception e){
			this.close(connection, statement, resultset);
			this.trackException(e);
		}
		return return_map;
	}
	
	public Map<String, Object> queryPage(String filelds,String table,String solution,int pageNo,boolean haswhere){
		this.clearTempStatement();
		Connection connection = null;
		PreparedStatement statement = null;
		ResultSet resultset = null;
		Map<String, Object> return_map = null;
		PageUtil page = null;
		String xp = " where ";
		if(haswhere){
			xp = " where ";
		}else{
			xp = " ";
		}
		try{
			return_map = new HashMap<String, Object>();
			List<Map<String, Object>> list = new ArrayList<Map<String, Object>>();
			
			connection = this.getConnection();
			if(connection == null){
				throw new Exception("db connection is null, please check db souce config currectly !");
			}
			String count_sql = "";
			if(solution == null){
				count_sql = "select count(*) as TT_NO from "+table;
			}else{
				count_sql = "select count(*) as TT_NO from "+table + " "+xp+" "+solution;
			}
			logger.debug(count_sql);
			statement = connection.prepareStatement(count_sql);
			resultset = statement.executeQuery();
			int count = 0;
			while(resultset.next()){
				count = resultset.getInt("TT_NO");
			}
			
			resultset.close();
			statement.close();
			
			if(count > 0){
				page = new PageUtil(count, pageNo);
				String query_sql = "";
				if(solution == null || solution.trim().length() == 0){
					query_sql = "select * from (select "+filelds+" from "+table+") where rownum<="+page.getEndRowNum()+" and rownum>="+page.getStartRowNum();
				}else{
					query_sql = "select y.* from (select x.*,rownum numbers from (select "+filelds+" from "+table+" "+xp+" "+solution+") x where rownum<="+page.getEndRowNum()+") y where y.numbers>="+page.getStartRowNum();
				}
				logger.debug(query_sql);
				statement = connection.prepareStatement(query_sql);
				resultset = statement.executeQuery();
				ResultSetMetaData rsmd = statement.getMetaData();
				int columnCount = rsmd.getColumnCount();
				
				
				
				while(resultset.next()){
					Map<String, Object> map = new HashMap<String, Object>();
					for(int i=1;i<=columnCount;i++){
						String type = rsmd.getColumnTypeName(i);
						String name = rsmd.getColumnName(i);
						if(type.equals("DATE")){
							String value = this.trackValue(resultset.getDate(name))+" "+this.trackValue(resultset.getTime(name));
							if(value.equals(" ")){
								value = "";
							}
							map.put(name, value);							
						}else{
							map.put(name, this.trackValue(resultset.getObject(name)));
						}
					}
					list.add(map);
				}				
				return_map.put(DBBuilder.ROW_COUNT, count);
				return_map.put(DBBuilder.PAGE_COUNT, page.getPageCount());
				return_map.put(DBBuilder.CURRENT_PAGE_NO, page.getCurrentPageNO());
			}else{
				return_map.put(DBBuilder.ROW_COUNT, 0);
				return_map.put(DBBuilder.PAGE_COUNT, 0);
				return_map.put(DBBuilder.CURRENT_PAGE_NO, 0);
			}
			return_map.put(DBBuilder.DATA_LIST, list);
			this.close(connection, statement, resultset);
		}catch(Exception e){
			this.close(connection, statement, resultset);
			this.trackException(e);
		}
		return return_map;
	}
	
	public int getCount(String table, String solution){
		this.clearTempStatement();
		Connection connection = null;
		PreparedStatement statement = null;
		ResultSet resultset = null;
		int index = -1;
		try{
			
			connection = this.getConnection();
			if(connection == null){
				throw new Exception("db connection is null, please check db souce config currectly !");
			}
			String sql = "";
			if(solution == null){
				sql = "select count(*) as TTT_NO from "+table;
			}else{
				sql = "select count(*) as TTT_NO from "+table+" where "+solution;
			}
			logger.debug(sql);
			statement = connection.prepareStatement(sql);
			resultset = statement.executeQuery();
			while(resultset.next()){
			    index = resultset.getInt("TTT_NO");
			}			 
			this.close(connection, statement, resultset);
		}catch(Exception e){
			this.close(connection, statement, resultset);
			this.trackException(e);
		}
		return index;
	}
	
	public int getCount(String table, String indexName,String solution){
		this.clearTempStatement();
		Connection connection = null;
		PreparedStatement statement = null;
		ResultSet resultset = null;
		int index = -1;
		try{
			connection = this.getConnection();
			if(connection == null){
				throw new Exception("db connection is null, please check db souce config currectly !");
			}
			String sql = "";
			if(solution == null){
				sql = "select count("+indexName+") as TTT_NO from "+table;
			}else{
				sql = "select count("+indexName+") as TTT_NO from "+table+" where "+solution;
			}
			logger.debug(sql);
			statement = connection.prepareStatement(sql);
			resultset = statement.executeQuery();
			while(resultset.next()){
			    index = resultset.getInt("TTT_NO");
			}			 
			this.close(connection, statement, resultset);
		}catch(Exception e){
			this.close(connection, statement, resultset);
			this.trackException(e);
		}
		return index;
	}
	
	public int getDistinctCount(String table, String indexName,String solution){
		this.clearTempStatement();
		Connection connection = null;
		PreparedStatement statement = null;
		ResultSet resultset = null;
		int index = -1;
		try{
			connection = this.getConnection();
			if(connection == null){
				throw new Exception("db connection is null, please check db souce config currectly !");
			}
			String sql = "";
			if(solution == null){
				sql = "select distinct count("+indexName+") as TTT_NO from "+table;
			}else{
				sql = "select distinct count("+indexName+") as TTT_NO from "+table+" where "+solution;
			}
			logger.debug(sql);
			statement = connection.prepareStatement(sql);
			resultset = statement.executeQuery();
			while(resultset.next()){
			    index = resultset.getInt("TTT_NO");
			}			 
			this.close(connection, statement, resultset);
		}catch(Exception e){
			this.close(connection, statement, resultset);
			this.trackException(e);
		}
		return index;
	}
	
	public List<Map<String, Object>> queryList(String sql){
		this.clearTempStatement();
		Connection connection = null;
		PreparedStatement statement = null;
		ResultSet resultset = null;
		List<Map<String, Object>> list = new ArrayList<Map<String, Object>>();
		try{
			list = new ArrayList<Map<String, Object>>();
			connection = this.getConnection();
			if(connection == null){
				throw new Exception("db connection is null, please check db souce config currectly !");
			}
			logger.debug(sql);
			statement = connection.prepareStatement(sql);
			resultset = statement.executeQuery();
			ResultSetMetaData rsmd = resultset.getMetaData();		
			int columnCount = rsmd.getColumnCount();
			while(resultset.next()){				
				Map<String, Object> map = new HashMap<String, Object>();				
				for(int i=1;i<=columnCount;i++){
					String columnName = rsmd.getColumnName(i);
					String columnType = rsmd.getColumnTypeName(i);	
					if(columnType.equals("DATE")){
						String value = this.trackValue(resultset.getDate(columnName)+" "+this.trackValue(resultset.getTime(columnName)));	
//						System.out.println("---------"+value);
						if(value.equals(" ")){
							value = "";
						}
						map.put(columnName, (String)value);
					}else{
						map.put(columnName, this.trackValue(resultset.getObject(columnName)));
					}		
				}
				list.add(map);		
			}
			this.close(connection, statement, resultset);
		}catch(Exception e){
			this.close(connection, statement, resultset);
			this.trackException(e);
		}
		return list;	
	}
	
	public List<String> queryListForSingleField(String column,String table,String solution){
		this.clearTempStatement();
		Connection connection = null;
		PreparedStatement statement = null;
		ResultSet resultset = null;
		List<String> list = new ArrayList<String>();
		try{
			list = new ArrayList<String>();
			connection = this.getConnection();
			if(connection == null){
				throw new Exception("db connection is null, please check db souce config currectly !");
			}
			String sql = "";
			if(solution == null){
				sql = "select distinct "+column+ " from "+table;
			}else{
				sql = "select distinct "+column+ " from "+table +" where "+solution;
			}
			logger.debug(sql);
			statement = connection.prepareStatement(sql);
			resultset = statement.executeQuery();
			ResultSetMetaData rsmd = resultset.getMetaData();		
			int columnCount = rsmd.getColumnCount();
			while(resultset.next()){
				String value = "";
				for(int i=1;i<=columnCount;i++){
					String columnName = rsmd.getColumnName(i);
					String columnType = rsmd.getColumnTypeName(i);	
					
					if(columnType.equals("DATE")){
						value = this.trackValue(resultset.getDate(columnName))+" "+this.trackValue(resultset.getTime(columnName));
						if(value.equals(" ")){
							value = "";
						}
					}else{
						value = this.trackValue(resultset.getObject(columnName));
					}		
				}
				list.add(value);		
			}
			this.close(connection, statement, resultset);
		}catch(Exception e){
			this.close(connection, statement, resultset);
			this.trackException(e);
		}
		return list;	
	}
	
	public List<String> queryListForDistinctSingleField(String column,String table,String solution){
		this.clearTempStatement();
		Connection connection = null;
		PreparedStatement statement = null;
		ResultSet resultset = null;
		List<String> list = new ArrayList<String>();
		try{
			list = new ArrayList<String>();
			connection = this.getConnection();
			if(connection == null){
				throw new Exception("db connection is null, please check db souce config currectly !");
			}
			String sql = "";
			if(solution == null){
				sql = "select distinct "+column+ " from "+table;
			}else{
				sql = "select distinct "+column+ " from "+table +" where "+solution;
			}
			logger.debug(sql);
			statement = connection.prepareStatement(sql);
			resultset = statement.executeQuery();
			ResultSetMetaData rsmd = resultset.getMetaData();		
			int columnCount = rsmd.getColumnCount();
			while(resultset.next()){
				String value = "";
				for(int i=1;i<=columnCount;i++){
					String columnName = rsmd.getColumnName(i);
					String columnType = rsmd.getColumnTypeName(i);	
					
					if(columnType.equals("DATE")){
						value = this.trackValue(resultset.getDate(columnName))+" "+this.trackValue(resultset.getTime(columnName));
						if(value.equals(" ")){
							value = "";
						}
					}else{
						value = this.trackValue(resultset.getObject(columnName));
					}		
				}
				list.add(value);		
			}
			this.close(connection, statement, resultset);
		}catch(Exception e){
			this.close(connection, statement, resultset);
			this.trackException(e);
		}
		return list;	
	}
	
	public Map<String, Object> queryMap(String sql){
		this.clearTempStatement();
		Connection connection = null;
		PreparedStatement statement = null;
		ResultSet resultset = null;
		Map<String, Object> map = null;
		try{	
			map = new HashMap<String, Object>();
			connection = this.getConnection();
			if(connection == null){
				throw new Exception("db connection is null, please check db souce config currectly !");
			}
			logger.debug(sql);
			statement = connection.prepareStatement(sql);
			resultset = statement.executeQuery();
			ResultSetMetaData rsmd = resultset.getMetaData();		
			int columnCount = rsmd.getColumnCount();
			while(resultset.next()){	
				
				for(int i=1;i<=columnCount;i++){
					String columnName = rsmd.getColumnName(i);
					String columnType = rsmd.getColumnTypeName(i);	
					if(columnType.equals("DATE")){
						String value = this.trackValue(resultset.getDate(columnName)+" "+this.trackValue(resultset.getTime(columnName)));	
						if(value.equals(" ")){
							value = "";
						}
						map.put(columnName, (String)value);
					}else{
						map.put(columnName, this.trackValue(resultset.getObject(columnName)));
					}		
				}
				break;	
			}
			this.close(connection, statement, resultset);
		}catch(Exception e){
			this.close(connection, statement, resultset);
			this.trackException(e);
		}
		if(map.size()>0){
			return map;
		}else{
			return null;
		}
	}
	
	private void clearTempStatement(){
		this.SQL_Error_Code = 0;
		this.SQL_Error_Message = null;
	}
	
	public void creatJ2EEBean(String beanpath,String packagepath){
		Connection connection = null;
		PreparedStatement statement = null;
		ResultSet resultset = null;
		
		File forder_java = new File(beanpath+"/java"); 
		if(!forder_java.exists()){
			forder_java.mkdir();
		}
		
		File forder_as = new File(beanpath+"/as"); 
		if(!forder_as.exists()){
			forder_as.mkdir();
		}
		
		try{
			connection = this.getConnection();
			if(connection == null){
				throw new Exception("db connection is null, please check db souce config currectly !");
			}
			statement = connection.prepareStatement("select tname from tab where tname not like 'BIN%'");
			resultset = statement.executeQuery();
			ResultSetMetaData rsmd = statement.getMetaData();
			int table_count = rsmd.getColumnCount();
			List<String> list_tab = new ArrayList<String>();
			while(resultset.next()){
				for(int i=1;i<=table_count;i++){
					list_tab.add(this.trackValue(resultset.getObject("TNAME")));
				}
			}
			
			for(int k=0;k<list_tab.size();k++){
				
				String table = (String) list_tab.get(k);
				
				FileUtil fu_java = new FileUtil(beanpath+"/java/"+table+".java");
				fu_java.creatFile();
				fu_java.writeFileContext("package "+packagepath+";\n", false);
				fu_java.writeFileContext("public class "+table+"{", false);
				
				FileUtil fu_as = new FileUtil(beanpath+"/as/"+table+".as");
				fu_as.creatFile();
				fu_as.writeFileContext("package "+packagepath+"{\n", false);
				fu_as.writeFileContext("\t[RemoteClass(alias=\"cn.ucans.bean."+table+"\")]", false);
				fu_as.writeFileContext("\tpublic class "+table+"{", false);
				
				statement = connection.prepareStatement("select * from "+table);
				resultset = statement.executeQuery();
				ResultSetMetaData rsmd_table = statement.getMetaData();
				int columnCount = rsmd_table.getColumnCount();
				
				for(int i=1;i<=columnCount;i++){
					String columnName = rsmd_table.getColumnName(i);
					fu_java.writeFileContext("\t\tpublic String "+columnName+" = \"\";", false);
					fu_as.writeFileContext("\t\tpublic var "+columnName+":String = \"\";", false);
				}
				fu_java.writeFileContext("}", false);
				fu_java.writeFileContext("", false);
				fu_as.writeFileContext("\t}", false);
				fu_as.writeFileContext("}", false);
			}
			this.close(connection, statement, resultset);
		}catch(Exception e){
			this.close(connection, statement, resultset);
			e.printStackTrace();
		}
	}
	
	public void close(Connection connection, Statement statement, ResultSet resultset){
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
			logger.debug("db release souce complete !");
		}catch(Exception e){
			if(resultset != null){
				resultset = null;
			}			
			if(statement != null){
				statement = null;
			}			
			if(connection != null){
				connection = null;
			}
			logger.error("database statement close failed,wait gc lose souce later \n"+e.getMessage());
		}
	}
	
	public void close(Connection connection, PreparedStatement statement, ResultSet resultset){
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
			logger.debug("db release souce complete !");
		}catch(Exception e){
			if(resultset != null){
				resultset = null;
			}			
			if(statement != null){
				statement = null;
			}			
			if(connection != null){
				connection = null;
			}
			logger.error("database statement close failed,wait gc lose souce later \n"+e.getMessage());
		}
	}
	
	public String trackValue(Object object){
		if(object == null || object.toString().trim().length()==0 || object.toString().trim().equals("null") || object.toString().trim().equals("NULL")){
			return "";
		}else{
			return object.toString();
		}
	}
	
	public String getTo_DateValue(String time,String format){
		return "to_date('"+time+"','"+format+"')";
	}
	
	public void trackException(Exception e){
		if(e instanceof SQLException){
			SQLException sql_exception = (SQLException) e; 
			this.SQL_Error_Code = sql_exception.getErrorCode();
			this.SQL_Error_Message = sql_exception.getMessage();
		}else{
			this.SQL_Error_Code = -1000000;
			this.SQL_Error_Message = e.getMessage();
		}
		logger.error(this.SQL_Error_Code+"<->"+this.SQL_Error_Message);
	}
	
	public String leadDataToFile(String[] columnNames, String sheetName, String sql){
		String url = null;
		WritableWorkbook wwb = null;
		WritableSheet ws = null;
		File folder = null;
		File file = null;
		Connection connection = null;
		PreparedStatement statement = null;
		ResultSet resultset = null;
		try{
			
			folder = new File(ChobitsConfig.TempFileForderPath);
			file = File.createTempFile("Excel",".xls",folder);
			wwb = Workbook.createWorkbook(file);
			ws = wwb.createSheet(sheetName, 0);
			
			for(int i=0;i<columnNames.length;i++){
				Label label_title = new Label(i, 0, columnNames[i]);
				ws.addCell(label_title);
			}
			
			connection = this.getConnection();
			statement = connection.prepareStatement(sql);
			resultset = statement.executeQuery();
			ResultSetMetaData rsmd = resultset.getMetaData();		
			int columnCount = rsmd.getColumnCount();
			int rowIndex = 1;
			while(resultset.next()){							
				for(int i=1;i<=columnCount;i++){
					String columnName = rsmd.getColumnName(i);
					String columnType = rsmd.getColumnTypeName(i);	
					String value = "";
					if(columnType.equals("DATE")){
						value = this.trackValue(resultset.getDate(columnName)+" "+resultset.getTime(columnName));
					}else{
						value = this.trackValue(resultset.getObject(columnName));
					}
					value = value.replaceAll(" ", "");
					value = value.replaceAll("null", "");
					Label label_value = new Label(i-1, rowIndex, value);
					ws.addCell(label_value);
				}
				rowIndex ++;				
			}
			wwb.write();
			wwb.close();
			this.close(connection, statement, resultset);
			url = ChobitsConfig.VisitURL+ "/TempFile/"+file.getName();
		}catch(Exception e){
			this.trackException(e);
			this.close(connection, statement, resultset);
		}
		return url;
	}
	
	public String leadDataToFile(String xml, String sheetName){
		String url = null;
		WritableWorkbook wwb = null;
		WritableSheet ws = null;
		File folder = null;
		File file = null;
		Connection connection = null;
		PreparedStatement statement = null;
		ResultSet resultset = null;
		try{
			
			folder = new File(ChobitsConfig.TempFileForderPath);
			file = File.createTempFile("Excel",".xls",folder);
			wwb = Workbook.createWorkbook(file);
			ws = wwb.createSheet(sheetName, 0);
			logger.debug(xml);
			XMLBuilder builder = new XMLBuilder(xml,XMLBuilder.INIT_WITHOUT_HEAD);
			Element element = builder.getRootElement();
			String title = element.getAttributeValue("title");
			String[] columnNames = title.split(",");	
			for(int i=0;i<columnNames.length;i++){
				Label label_title = new Label(i, 0, columnNames[i]);
				ws.addCell(label_title);
			}
			
			if(element.hasChildren()){
				List<Element> list_row = element.getChildren();
				for(int i=0;i<list_row.size();i++){
					Element item_elem = list_row.get(i);
					if(item_elem.hasChildren()){
						List<Element> list_cell = item_elem.getChildren();
						for(int k=0;k<list_cell.size();k++){
							Element cell = list_cell.get(k);
							Label label_value = new Label(k, i+1, cell.getText());
							ws.addCell(label_value);
						}
					}		
				}
			}
			
			wwb.write();
			wwb.close();
			this.close(connection, statement, resultset);
			url = ChobitsConfig.VisitURL+ "/TempFile/"+file.getName();
		}catch(Exception e){
			this.trackException(e);
			this.close(connection, statement, resultset);
		}
		return url;
	}
	
	public String leadDataToFile(String[] columnNames, String sheetName, ArrayList<Map<String,Object>> list){
		String url = null;
		jxl.write.WritableWorkbook wwb = null;
		jxl.write.WritableSheet ws = null;
		File folder = null;
		File file = null;
		
		try{
			folder = new File(ChobitsConfig.TempFileForderPath);
			file = File.createTempFile("Excel",".xls",folder);
			wwb = Workbook.createWorkbook(file);
			ws = wwb.createSheet(sheetName, 0);
			int columnCount = columnNames.length;
			for(int i=0;i<columnNames.length;i++){
				Label label_title = new Label(i, 0, columnNames[i]);
				ws.addCell(label_title);
			}
		
			for(int i=0;i<list.size();i++){
				Map<String,Object> map = list.get(i);
				Iterator iterator = map.keySet().iterator(); 
				int index = 0;	
				while(iterator.hasNext()){									
					Object key = iterator.next(); 
					String value = this.trackValue(map.get(key)); 
					Label label_value = new Label(index, i+1, value);
					ws.addCell(label_value);
					index ++;
				}				
			}
			wwb.write();
			wwb.close();
			url = ChobitsConfig.VisitURL+ "/TempFile/"+file.getName();
		}catch(Exception e){
			if(wwb != null){
				try {
					wwb.close();
				} catch (Exception e1) {
					logger.error("导出文件"+url+",\n关闭JXL写入流出错，原因是："+e1.getMessage());
					wwb = null;
				}
			}
			this.trackException(e);
		}
		return url;
	}
	
	public String nextMaxValue(String table,String column){
		return "(select max("+column+")+1 from "+table+")";
	}
	
	public Object queryObjectClumn(String sql){
		
		this.clearTempStatement();
		Object returnN=0;
		Connection connection  = null;
		PreparedStatement statement = null;
		ResultSet resultset = null;
		try{
			logger.debug(sql);
			connection = this.getConnection();
			statement = connection.prepareStatement(sql);
			resultset = statement.executeQuery();
			if(resultset.next()){
				returnN=resultset.getObject(1);
			}
			this.close(connection, statement, resultset);
		}catch(Exception e){
			this.trackException(e);
		}
		return returnN;	
	}
	
	public static void main(String[] args) throws Exception{
		DBBuilder.initCommonType();
		DBBuilder builder = new DBBuilder();
//		builder.excute("insert into T_Terminal_Change_His(Imei_Number,Uim_Number,Msisdn,Change_Type,User_ID,Change_Time) values ('666666666666666','666666666666666','66666666666','01','admin',sysdate)");
//		System.out.println(builder.SQL_Error_Code+","+builder.SQL_Error_Message);
		//builder.creatJ2EEBean("C:/beans","cn.ucans.bean");
		
//		for(long i=0;i<60;i++){
//			long imi_no = 990899089908009L;
//			long umi_no = 330833083308009L;
//			long phone = 13910526499L;
//
//			String sql = "insert into T_Service_Cash_200902 values (Bill_SEQ.nextVal,'"+imi_no+"','"+umi_no+"','"+phone+"','1345','NNNNN','1200',221000,null,to_date('2009-1-18 13:00:00','yyyy-mm-dd hh24:mi:ss'),to_date('2009-1-18 13:20:00','yyyy-mm-dd hh24:mi:ss'),to_date('2009-1-18 13:22:00','yyyy-mm-dd hh24:mi:ss'),3512,3001,'0','0','eludar',null,null,null)";
//			builder.excute(sql);
//			System.out.println(sql);
//			if(builder.SQL_Error_Code != 0){
//				throw new Exception(builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
//			}
//		}
//		Role_ID						Varchar2(16),		--角色ID
//		Menu_ID						Number(9),			--菜单编号
//		Menu_Name					Varchar2(50),		--菜单名称
//		Top_Level					Number(1),			--菜单层次(从0开始)
//		Program_Name				Varchar2(60),		--程序块名称
//		Parent_ID					Number(9),			--父层菜单编号
//		Menu_Notes					Varchar2(128)		--菜单说明
		List<Map<String,Object>> list = builder.queryList("select Role_ID,Menu_ID,Menu_Name,Top_Level,Program_Name,Parent_ID,Menu_Notes from T_Role_Menu where Role_ID='00A' order by Menu_ID");
		for(int i=0;i<list.size();i++){
			Map<String,Object> map = list.get(i);
			String Role_ID = (String) map.get("ROLE_ID");
			String Menu_ID = (String) map.get("MENU_ID");
			String Menu_Name = (String) map.get("MENU_NAME");
			String Top_Level = (String) map.get("TOP_LEVEL");
			String Program_Name = (String) map.get("PROGRAM_NAME");
			String Parent_ID = (String) map.get("PARENT_ID");
			String Menu_Notes = (String) map.get("MENU_NOTES");
			
			String sql = "insert into T_Role_Menu (Role_ID,Menu_ID,Menu_Name,Top_Level,Program_Name,Parent_ID,Menu_Notes) values ";
			sql += "('"+Role_ID+"',"+Menu_ID+",'"+Menu_Name+"',"+Top_Level+",'"+Program_Name+"',"+Parent_ID+",'"+Menu_Notes+"');";
			
			System.out.println(sql);
		}
	}
}
