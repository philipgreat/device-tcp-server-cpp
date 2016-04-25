package chobits.db;
import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.Set;



public class TransformChinese
{
	private static LinkedHashMap spellMap = null;
	static
	{
		if(spellMap == null)
		{
			spellMap = new LinkedHashMap(400);
		}
		initialize();
	}
	public static String Uni2GB(String Uni)
	/*
		
	*/
	{
		try
		{
			byte[] tmpbyte = Uni.getBytes("ISO8859_1"); 
			return new String(tmpbyte); 
		}
		catch(Exception e)
		{
			return null;			
		}
		
	}


	public static String GB2Uni(String GB)
	/*
		
	*/
	{
		try
		{
		    byte[] tmp = GB.getBytes("GBK");
		    String result = new String(tmp, "ISO8859_1");
		    return result;
		}
		catch(Exception e)
		{
			return null;
		}
	}


	public static String toUtf8String(String s) 
	{
		StringBuffer sb = new StringBuffer();
		for (int i=0;i<s.length();i++) 
		{
			char c = s.charAt(i);
	        if (c >= 0 && c <= 255) 
	        {
	            sb.append(c);
	        } 
	        else 
	        {
	        	byte[] b;
	        	try 
	        	{
	        		b = Character.toString(c).getBytes("utf-8");
	            }
	        	catch (Exception ex)
	        	{
	                b = new byte[0];
	            }
	            for (int j = 0; j < b.length; j++) 
	            {
	                int k = b[j];
	                if (k < 0) k += 256;
	                sb.append("%" + Integer.toHexString(k).
	                toUpperCase());
	            }
	        }
	    }
		return sb.toString();
	}
	
	public static String Nvl(String strSour,String sTarg)
	{
		if("".equals(strSour))
		{
			return sTarg;
		}
		if((strSour == null)||(strSour.equals(""))||(strSour == ""))
		{
			return sTarg;
		}
		return strSour;
	}

	public static String unescape(String src)
	{
		StringBuffer tmp = new StringBuffer();
		tmp.ensureCapacity(src.length());
		int  lastPos=0,pos=0;
		char ch;
		while (lastPos<src.length())
		{
			pos = src.indexOf("%",lastPos);
			if (pos == lastPos)
			{
				if (src.charAt(pos+1)=='u')
				{
					ch = (char)Integer.parseInt(src.substring(pos+2,pos+6),16);
					tmp.append(ch);
					lastPos = pos+6;
				}
				else
				{
					ch = (char)Integer.parseInt(src.substring(pos+1,pos+3),16);
					tmp.append(ch);
					lastPos = pos+3;
				}
		   }
		   else
		   {
				if (pos == -1)
				{
					tmp.append(src.substring(lastPos));
					lastPos=src.length();
				}
				else
				{
					tmp.append(src.substring(lastPos,pos));
					lastPos=pos;
				}
			}
		}
		return tmp.toString();
	}
	
	public static String escape(String src)
	{
		int i;
		char j;
		StringBuffer tmp = new StringBuffer();
		tmp.ensureCapacity(src.length()*6);
		for (i=0;i<src.length() ;i++ )
		{
	 		j = src.charAt(i);
	 		if (Character.isDigit(j) || Character.isLowerCase(j) || Character.isUpperCase(j))
	 		{
	 			tmp.append(j);
	 		}
	 		else
	 		{
	 			if (j<256)
	 			{
	  				tmp.append( "%" );
	  				if (j<16)
	   					tmp.append( "0" );
	  				tmp.append( Integer.toString(j,16) );
	 			}
	 			else
	 			{
					tmp.append( "%u" );
					tmp.append( Integer.toString(j,16) );
				}
			}
		}
		return tmp.toString();
	}

	public static String getSpellByAscii(int ascii)
	{
		if(ascii > 0 && ascii < 160)
		{
			return String.valueOf((char)ascii);
		}
		if(ascii < -20319 || ascii > -10247)
		{
			return null;
		}
		Set keySet = spellMap.keySet();
		Iterator it = keySet.iterator();
		String spell0 = null;;
		String spell = null;
		int asciiRang0 = -20319;
		int asciiRang;
		while(it.hasNext())
		{
			spell = (String)it.next();
			Object valObj = spellMap.get(spell);
			if(valObj instanceof Integer)
			{
				asciiRang = ((Integer)valObj).intValue();
				if(ascii >= asciiRang0 && ascii < asciiRang)
				{
					return(spell0 == null) ? spell : spell0;
				}
				else
				{
					spell0 = spell;
					asciiRang0 = asciiRang;
				}
			}
		}
		return null;
	}
	
	public static int getCnAscii(char cn)
	{
		byte[] bytes = (String.valueOf(cn)).getBytes();
		if(bytes == null || bytes.length > 2 || bytes.length <= 0)
		{ 
			return 0;
		}
		if(bytes.length == 1)
		{
			return bytes[0];
		}
		if(bytes.length == 2)
		{
			int hightByte = 256 + bytes[0];
			int lowByte = 256 + bytes[1];
			int ascii = (256 * hightByte + lowByte) - 256 * 256;

			return ascii;
		}
		return 0;
	}
	
	private static void spellPut(int ascii,String spell)
	{
		spellMap.put(spell,new Integer(ascii));
	}

	public static String getFullSpell(String cnStr)
	{
		if(null == cnStr || "".equals(cnStr.trim()))
		{
			return cnStr;
		}
		char[] chars = cnStr.toCharArray();
		StringBuffer retuBuf = new StringBuffer();
		for(int i = 0,Len = chars.length;i < Len;i++)
		{
			int ascii = getCnAscii(chars[i]);
			if(ascii == 0)
			{
				retuBuf.append(chars[i]);
			}
			else
			{
				String spell = getSpellByAscii(ascii);
				if(spell == null)
				{
					retuBuf.append(chars[i]);
				}
				else
				{
					retuBuf.append(spell);
				}
			}
		}
		return retuBuf.toString();
	}
	
	public static String getFirstSpell(String cnStr)
	{
		if(null == cnStr || "".equals(cnStr.trim()))
		{
			return cnStr;
		}
		char[] chars = cnStr.toCharArray();
		StringBuffer retuBuf = new StringBuffer();
		for(int i = 0,Len = chars.length;i < Len;i++)
		{
			int ascii = getCnAscii(chars[i]);
			if(ascii == 0)
			{
				retuBuf.append(chars[i]);
			}
			else
			{
				String spell = getSpellByAscii(ascii);
				if(spell == null)
				{
					retuBuf.append('*');
				}
				else
				{
					retuBuf.append(spell.substring(0,1));
				}
			}
		}
		return retuBuf.toString();
	}
		
	private static void initialize()
	{
		spellPut(-20319,"a");
		spellPut(-20317,"ai");
		spellPut(-20304,"an");
		spellPut(-20295,"ang");
		spellPut(-20292,"ao");
		spellPut(-20283,"ba");
		spellPut(-20265,"bai");
		spellPut(-20257,"ban");
		spellPut(-20242,"bang");
		spellPut(-20230,"bao");
		spellPut(-20051,"bei");
		spellPut(-20036,"ben");
		spellPut(-20032,"beng");
		spellPut(-20026,"bi");
		spellPut(-20002,"bian");
		spellPut(-19990,"biao");
		spellPut(-19986,"bie");
		spellPut(-19982,"bin");
		spellPut(-19976,"bing");
		spellPut(-19805,"bo");
		spellPut(-19784,"bu");
		spellPut(-19775,"ca");
		spellPut(-19774,"cai");
		spellPut(-19763,"can");
		spellPut(-19756,"cang");
		spellPut(-19751,"cao");
		spellPut(-19746,"ce");
		spellPut(-19741,"ceng");
		spellPut(-19739,"cha");
		spellPut(-19728,"chai");
		spellPut(-19725,"chan");
		spellPut(-19715,"chang");
		spellPut(-19540,"chao");
		spellPut(-19531,"che");
		spellPut(-19525,"chen");
		spellPut(-19515,"cheng");
		spellPut(-19500,"chi");
		spellPut(-19484,"chong");
		spellPut(-19479,"chou");
		spellPut(-19467,"chu");
		spellPut(-19289,"chuai");
		spellPut(-19288,"chuan");
		spellPut(-19281,"chuang");
		spellPut(-19275,"chui");
		spellPut(-19270,"chun");
		spellPut(-19263,"chuo");
		spellPut(-19261,"ci");
		spellPut(-19249,"cong");
		spellPut(-19243,"cou");
		spellPut(-19242,"cu");
		spellPut(-19238,"cuan");
		spellPut(-19235,"cui");
		spellPut(-19227,"cun");
		spellPut(-19224,"cuo");
		spellPut(-19218,"da");
		spellPut(-19212,"dai");
		spellPut(-19038,"dan");
		spellPut(-19023,"dang");
		spellPut(-19018,"dao");
		spellPut(-19006,"de");
		spellPut(-19003,"deng");
		spellPut(-18996,"di");
		spellPut(-18977,"dian");
		spellPut(-18961,"diao");
		spellPut(-18952,"die");
		spellPut(-18783,"ding");
		spellPut(-18774,"diu");
		spellPut(-18773,"dong");
		spellPut(-18763,"dou");
		spellPut(-18756,"du");
		spellPut(-18741,"duan");
		spellPut(-18735,"dui");
		spellPut(-18731,"dun");
		spellPut(-18722,"duo");
		spellPut(-18710,"e");
		spellPut(-18697,"en");
		spellPut(-18696,"er");
		spellPut(-18526,"fa");
		spellPut(-18518,"fan");
		spellPut(-18501,"fang");
		spellPut(-18490,"fei");
		spellPut(-18478,"fen");
		spellPut(-18463,"feng");
		spellPut(-18448,"fo");
		spellPut(-18447,"fou");
		spellPut(-18446,"fu");
		spellPut(-18239,"ga");
		spellPut(-18237,"gai");
		spellPut(-18231,"gan");
		spellPut(-18220,"gang");
		spellPut(-18211,"gao");
		spellPut(-18201,"ge");
		spellPut(-18184,"gei");
		spellPut(-18183,"gen");
		spellPut(-18181,"geng");
		spellPut(-18012,"gong");
		spellPut(-17997,"gou");
		spellPut(-17988,"gu");
		spellPut(-17970,"gua");
		spellPut(-17964,"guai");
		spellPut(-17961,"guan");
		spellPut(-17950,"guang");
		spellPut(-17947,"gui");
		spellPut(-17931,"gun");
		spellPut(-17928,"guo");
		spellPut(-17922,"ha");
		spellPut(-17759,"hai");
		spellPut(-17752,"han");
		spellPut(-17733,"hang");
		spellPut(-17730,"hao");
		spellPut(-17721,"he");
		spellPut(-17703,"hei");
		spellPut(-17701,"hen");
		spellPut(-17697,"heng");
		spellPut(-17692,"hong");
		spellPut(-17683,"hou");
		spellPut(-17676,"hu");
		spellPut(-17496,"hua");
		spellPut(-17487,"huai");
		spellPut(-17482,"huan");
		spellPut(-17468,"huang");
		spellPut(-17454,"hui");
		spellPut(-17433,"hun");
		spellPut(-17427,"huo");
		spellPut(-17417,"ji");
		spellPut(-17202,"jia");
		spellPut(-17185,"jian");
		spellPut(-16983,"jiang");
		spellPut(-16970,"jiao");
		spellPut(-16942,"jie");
		spellPut(-16915,"jin");
		spellPut(-16733,"jing");
		spellPut(-16708,"jiong");
		spellPut(-16706,"jiu");
		spellPut(-16689,"ju");
		spellPut(-16664,"juan");
		spellPut(-16657,"jue");
		spellPut(-16647,"jun");
		spellPut(-16474,"ka");
		spellPut(-16470,"kai");
		spellPut(-16465,"kan");
		spellPut(-16459,"kang");
		spellPut(-16452,"kao");
		spellPut(-16448,"ke");
		spellPut(-16433,"ken");
		spellPut(-16429,"keng");
		spellPut(-16427,"kong");
		spellPut(-16423,"kou");
		spellPut(-16419,"ku");
		spellPut(-16412,"kua");
		spellPut(-16407,"kuai");
		spellPut(-16403,"kuan");
		spellPut(-16401,"kuang");
		spellPut(-16393,"kui");
		spellPut(-16220,"kun");
		spellPut(-16216,"kuo");
		spellPut(-16212,"la");
		spellPut(-16205,"lai");
		spellPut(-16202,"lan");
		spellPut(-16187,"lang");
		spellPut(-16180,"lao");
		spellPut(-16171,"le");
		spellPut(-16169,"lei");
		spellPut(-16158,"leng");
		spellPut(-16155,"li");
		spellPut(-15959,"lia");
		spellPut(-15958,"lian");
		spellPut(-15944,"liang");
		spellPut(-15933,"liao");
		spellPut(-15920,"lie");
		spellPut(-15915,"lin");
		spellPut(-15903,"ling");
		spellPut(-15889,"liu");
		spellPut(-15878,"long");
		spellPut(-15707,"lou");
		spellPut(-15701,"lu");
		spellPut(-15681,"lv");
		spellPut(-15667,"luan");
		spellPut(-15661,"lue");
		spellPut(-15659,"lun");
		spellPut(-15652,"luo");
		spellPut(-15640,"ma");
		spellPut(-15631,"mai");
		spellPut(-15625,"man");
		spellPut(-15454,"mang");
		spellPut(-15448,"mao");
		spellPut(-15436,"me");
		spellPut(-15435,"mei");   
		spellPut(-15419,"men");
		spellPut(-15416,"meng");
		spellPut(-15408,"mi");
		spellPut(-15394,"mian");
		spellPut(-15385,"miao");
		spellPut(-15377,"mie");
		spellPut(-15375,"min");
		spellPut(-15369,"ming");
		spellPut(-15363,"miu");
		spellPut(-15362,"mo");
		spellPut(-15183,"mou");
		spellPut(-15180,"mu");
		spellPut(-15165,"na");
		spellPut(-15158,"nai");
		spellPut(-15153,"nan");
		spellPut(-15150,"nang");
		spellPut(-15149,"nao");
		spellPut(-15144,"ne");
		spellPut(-15143,"nei");
		spellPut(-15141,"nen");
		spellPut(-15140,"neng");
		spellPut(-15139,"ni");
		spellPut(-15128,"nian");
		spellPut(-15121,"niang");
		spellPut(-15119,"niao");
		spellPut(-15117,"nie");
		spellPut(-15110,"nin");
		spellPut(-15109,"ning");
		spellPut(-14941,"niu");
		spellPut(-14937,"nong");
		spellPut(-14933,"nu");
		spellPut(-14930,"nv");
		spellPut(-14929,"nuan");
		spellPut(-14928,"nue");
		spellPut(-14926,"nuo");
		spellPut(-14922,"o");
		spellPut(-14921,"ou");
		spellPut(-14914,"pa");
		spellPut(-14908,"pai");
		spellPut(-14902,"pan");
		spellPut(-14894,"pang");
		spellPut(-14889,"pao");
		spellPut(-14882,"pei");
		spellPut(-14873,"pen");
		spellPut(-14871,"peng");
		spellPut(-14857,"pi");
		spellPut(-14678,"pian");
		spellPut(-14674,"piao");
		spellPut(-14670,"pie");
		spellPut(-14668,"pin");
		spellPut(-14663,"ping");
		spellPut(-14654,"po");
		spellPut(-14645,"pu");
		spellPut(-14630,"qi");
		spellPut(-14594,"qia");
		spellPut(-14429,"qian");
		spellPut(-14407,"qiang");
		spellPut(-14399,"qiao");
		spellPut(-14384,"qie");
		spellPut(-14379,"qin");
		spellPut(-14368,"qing");
		spellPut(-14355,"qiong");
		spellPut(-14353,"qiu");
		spellPut(-14345,"qu");
		spellPut(-14170,"quan");
		spellPut(-14159,"que");
		spellPut(-14151,"qun");
		spellPut(-14149,"ran");
		spellPut(-14145,"rang");
		spellPut(-14140,"rao");
		spellPut(-14137,"re");
		spellPut(-14135,"ren");
		spellPut(-14125,"reng");
		spellPut(-14123,"ri");
		spellPut(-14122,"rong");
		spellPut(-14112,"rou");
		spellPut(-14109,"ru");
		spellPut(-14099,"ruan");
		spellPut(-14097,"rui");
		spellPut(-14094,"run");
		spellPut(-14092,"ruo");
		spellPut(-14090,"sa");
		spellPut(-14087,"sai");
		spellPut(-14083,"san");
		spellPut(-13917,"sang");
		spellPut(-13914,"sao");
		spellPut(-13910,"se");
		spellPut(-13907,"sen");
		spellPut(-13906,"seng");
		spellPut(-13905,"sha");
		spellPut(-13896,"shai");
		spellPut(-13894,"shan");
		spellPut(-13878,"shang");
		spellPut(-13870,"shao");
		spellPut(-13859,"she");
		spellPut(-13847,"shen");
		spellPut(-13831,"sheng");
		spellPut(-13658,"shi");
		spellPut(-13611,"shou");
		spellPut(-13601,"shu");
		spellPut(-13406,"shua");
		spellPut(-13404,"shuai");
		spellPut(-13400,"shuan");
		spellPut(-13398,"shuang");
		spellPut(-13395,"shui");
		spellPut(-13391,"shun");
		spellPut(-13387,"shuo");
		spellPut(-13383,"si");
		spellPut(-13367,"song");
		spellPut(-13359,"sou");
		spellPut(-13356,"su");
		spellPut(-13343,"suan");
		spellPut(-13340,"sui");
		spellPut(-13329,"sun");
		spellPut(-13326,"suo");
		spellPut(-13318,"ta");
		spellPut(-13147,"tai");
		spellPut(-13138,"tan");
		spellPut(-13120,"tang");
		spellPut(-13107,"tao");
		spellPut(-13096,"te");
		spellPut(-13095,"teng");
		spellPut(-13091,"ti");
		spellPut(-13076,"tian");
		spellPut(-13068,"tiao");
		spellPut(-13063,"tie");
		spellPut(-13060,"ting");
		spellPut(-12888,"tong");
		spellPut(-12875,"tou");
		spellPut(-12871,"tu");
		spellPut(-12860,"tuan");
		spellPut(-12858,"tui");
		spellPut(-12852,"tun");
		spellPut(-12849,"tuo");
		spellPut(-12838,"wa");
		spellPut(-12831,"wai");
		spellPut(-12829,"wan");
		spellPut(-12812,"wang");
		spellPut(-12802,"wei");
		spellPut(-12607,"wen");
		spellPut(-12597,"weng");
		spellPut(-12594,"wo");
		spellPut(-12585,"wu");
		spellPut(-12556,"xi");
		spellPut(-12359,"xia");
		spellPut(-12346,"xian");
		spellPut(-12320,"xiang");
		spellPut(-12300,"xiao");
		spellPut(-12120,"xie");
		spellPut(-12099,"xin");
		spellPut(-12089,"xing");
		spellPut(-12074,"xiong");
		spellPut(-12067,"xiu");
		spellPut(-12058,"xu");
		spellPut(-12039,"xuan");
		spellPut(-11867,"xue");
		spellPut(-11861,"xun");
		spellPut(-11847,"ya");
		spellPut(-11831,"yan");
		spellPut(-11798,"yang");
		spellPut(-11781,"yao");
		spellPut(-11604,"ye");
		spellPut(-11589,"yi");
		spellPut(-11536,"yin");
		spellPut(-11358,"ying");
		spellPut(-11340,"yo");
		spellPut(-11339,"yong");
		spellPut(-11324,"you");
		spellPut(-11303,"yu");
		spellPut(-11097,"yuan");
		spellPut(-11077,"yue");
		spellPut(-11067,"yun");
		spellPut(-11055,"za");
		spellPut(-11052,"zai");
		spellPut(-11045,"zan");
		spellPut(-11041,"zang");
		spellPut(-11038,"zao");
		spellPut(-11024,"ze");
		spellPut(-11020,"zei");
		spellPut(-11019,"zen");
		spellPut(-11018,"zeng");
		spellPut(-11014,"zha");
		spellPut(-10838,"zhai");
		spellPut(-10832,"zhan");
		spellPut(-10815,"zhang");
		spellPut(-10800,"zhao");
		spellPut(-10790,"zhe");
		spellPut(-10780,"zhen");
		spellPut(-10764,"zheng");
		spellPut(-10587,"zhi");
		spellPut(-10544,"zhong");
		spellPut(-10533,"zhou");
		spellPut(-10519,"zhu");
		spellPut(-10331,"zhua");
		spellPut(-10329,"zhuai");
		spellPut(-10328,"zhuan");
		spellPut(-10322,"zhuang");
		spellPut(-10315,"zhui");
		spellPut(-10309,"zhun");
		spellPut(-10307,"zhuo");
		spellPut(-10296,"zi");
		spellPut(-10281,"zong");
		spellPut(-10274,"zou");
		spellPut(-10270,"zu");
		spellPut(-10262,"zuan");
		spellPut(-10260,"zui");
		spellPut(-10256,"zun");
		spellPut(-10254,"zuo");
		spellPut(-10247,"zz");
	}
/*	
	public static void main(String args[])
	{
		String	strData;
		strData = TransformChinese.getFullSpell(args[0]);
		System.out.println(strData);
	}
*/
}