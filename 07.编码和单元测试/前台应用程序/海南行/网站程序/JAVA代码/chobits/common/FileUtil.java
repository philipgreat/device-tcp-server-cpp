package chobits.common;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

public class FileUtil {
	
	private String link = null;
	
	public FileUtil(){}
	
	public FileUtil(String linkPath){
		this.link = linkPath;
	}
	
	public void creatFile(String linkPath){
		BufferedWriter out = null;
		try{
			out = new BufferedWriter(new FileWriter(linkPath));
			out.write("");
			out.close();
		}catch (IOException e){
			e.printStackTrace();
		}
	}
	
	public void creatFile(){
		BufferedWriter out = null;
		try{
			out = new BufferedWriter(new FileWriter(this.link));
			out.write("");
			out.close();
		}catch (IOException e){
			e.printStackTrace();
		}
	}
	
	public void overwriteToFile(String linkPath,String context,boolean encoded){
		BufferedWriter out = null;
		try{
			out = new BufferedWriter(new FileWriter(linkPath));
			if(encoded){
				context = new String(context.getBytes("GBK"), "ISO-8859-1");
			}
			out.write(context);
			out.close();
		}catch (IOException e){
			e.printStackTrace();
		}
	}
	
	public void overwriteToXMLFile(String linkPath,String context,boolean encoded){
		BufferedWriter out = null;
		try{
			out = new BufferedWriter(new FileWriter(linkPath));
			if(encoded){
				context = new String(context.getBytes("GBK"), "UTF-8");
			}
			out.write(context);
			out.close();
		}catch (IOException e){
			e.printStackTrace();
		}
	}
	
	public void writeFileContext(String linkPath,String context,boolean encoded){
		RandomAccessFile rf = null;
		try{
			rf = new RandomAccessFile(linkPath, "rw");
			rf.seek(rf.length());
			if(encoded){
				context = new String(context.getBytes("GBK"), "ISO-8859-1");
			}			
			rf.writeBytes(context+"\n");
			rf.close();
		}catch (Exception e){
			e.printStackTrace();
		}
	}
	
	public void writeFileContext(String context,boolean encoded){
		RandomAccessFile rf = null;
		try{
			rf = new RandomAccessFile(this.link, "rw");
			rf.seek(rf.length());
			if(encoded){
				context = new String(context.getBytes("GBK"), "ISO-8859-1");
			}			
			rf.writeBytes(context+"\n");
			rf.close();
		}catch (Exception e){
			e.printStackTrace();
		}
	}
	
	public String copyFile(String old_path,String new_path){
		String name = null;
		try{
			BufferedReader br = new BufferedReader(new FileReader(old_path));   
			BufferedWriter bw = new BufferedWriter(new FileWriter(new_path));   
			String s,space="     ";   
			int num=0;   
			while(br.ready()){   
				s=br.readLine();   
				num++;   
				bw.write(String.valueOf(num));   
				bw.write(space);   
				bw.write(s);   
				bw.newLine();   
			}   
			bw.close();
			br.close();
			File file = new File(new_path);
			if(file.exists()){
				name = file.getName();
			}
		}catch(Exception e){
			e.printStackTrace();
		}
	    return name;
	}
	
	public boolean zipFile(String file,String temp_forder,String zipFileName){
		boolean ok = false;
		try{
			FileInputStream in = new FileInputStream(file);
			FileOutputStream out = new FileOutputStream(temp_forder+"/"+zipFileName);  // 1
			ZipOutputStream zipOut = new ZipOutputStream(out);
			File file_link = new File(file);
			ZipEntry entry = new ZipEntry(file_link.getName());
			zipOut.putNextEntry(entry);
			int nNumber;
			byte[] buffer = new byte [512];
			while((nNumber=in.read(buffer)) != -1)
			zipOut.write(buffer,0 ,nNumber);
			zipOut.close();
			out.close();
			in.close();		
			ok = true;
		}catch(Exception e){
			e.printStackTrace();
		}
		return ok;
	}
}
