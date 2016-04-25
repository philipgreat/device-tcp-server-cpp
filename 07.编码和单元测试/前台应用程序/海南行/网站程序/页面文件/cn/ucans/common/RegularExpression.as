package cn.ucans.common
{
	import flash.events.KeyboardEvent;
	import flash.ui.Keyboard;
	
	
	public class RegularExpression
	{
		public var strAlertCode:String = "";
		public function RegularExpression()
		{
		}
		
		public function isEmail(item:String):Boolean{
			var expression:RegExp = /\w+([-+.]\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*/
			return expression.exec(item);
		}
		
		public function isChineseCharator(item:String):Boolean{
			var expression:RegExp = /[\u4e00-\u9fa5]/
			return expression.exec(item);
		}
		
		
		
		public function isContainTrim(item:String):Boolean{
			var expression:RegExp = /^\s*|\s*$/
			return expression.exec(item);
		}
		
		
		
		public function isChinaFixedLineTelephone (item:String):Boolean{
			var expression:RegExp = / \d{3,4}-\d{7,8}|(\d{11})/
			return expression.exec(item);
		}
		
		public function isQQ(item:String):Boolean{
			var expression:RegExp = /[1-9][0-9]{4,}/
			return expression.exec(item);
		}
		
		public function isChinaIDCard(item:String):Boolean{
			var expression:RegExp = /\d{15}|\d{18}/
			return expression.exec(item);
		}
		
		public function isChinaPostCode(item:String):Boolean{
//			if(item.length == 6 && this.isNumber(item)){
//				return true;
//			}else{
//				return false;
//			}
			var expression:RegExp = /^[0-9][0-9]{5}$/
			return expression.exec(item);
		}
		
		public function isIP(item:String):Boolean{
			var expression:RegExp = /\d+\.\d+\.\d+\.\d+/
			return expression.exec(item);
		}
		
		public function isChineseMobileTelephone(item:String):Boolean{
			var isok:Boolean = false;
			if(item.length == 11 && isNumber(item)){
				isok = true;
			}
//			var expression:RegExp = /^((\(\d{3}\))|(\d{3}\-))?13[0123456789]\d{8}|15[89]\d{8}/;
//			return expression.exec(item);
			return isok;
		}
		
		public function isCarID(item:String):Boolean{
			if(item.length<20){
				return true;
			}else{
				return false;
			}
		}
		
//		public function isNumber(item:String):Boolean{
//			var expression:RegExp = /^[0-9]\d*$/;
//			return expression.exec(this.trim(item));
//		}
		
		public function isNumber(item:String):Boolean{
			var expression:RegExp = /^(-?\d+)(\.|\.\d+)?$/;
			return expression.exec(this.trim(item));
		}
		
		public function isNumberORCharacter(item:String):Boolean{
			var expression:RegExp = /^[A-Za-z0-9]+$/;
			return expression.exec(this.trim(item));
		}
		
		public function isLengthSizeOK(item:String,size:int):Boolean{
			if(this.trim(item).length == size){
				return true;
			}else{
				return false;
			
			}
		}
		
		public function trackNumber(item:Object,ratio:Number):String{
			if(item == null || this.trim(item.toString()).length==0){
				return "";
			}else{
				return ""+Number(item)/ratio;
			}
		}
		
		public function trim(item:String):String{
			return item.replace(/(^\s*)|(\s*$)/g, "");

		}
		
		public function nvl(item:String):String{
			if(item == null || this.trim(item).length==0){
				return "";
			}else{
				return item.toString();
			}
		}
		
		public static function fun_Check(event:KeyboardEvent):void{
				var		nLen:int = 0;
				var 	n:int = 0;
				var		nCount:int = 0;
				if(event.keyCode == Keyboard.RIGHT || event.keyCode == Keyboard.DOWN ||
					event.keyCode == Keyboard.BACKSPACE || event.keyCode == Keyboard.LEFT ||
					event.keyCode == Keyboard.UP || event.keyCode == Keyboard.DELETE || 
					event.keyCode == Keyboard.CAPS_LOCK || event.keyCode == Keyboard.TAB ||
					event.keyCode == Keyboard.CONTROL||event.keyCode == Keyboard.SHIFT)
				{
					return;
				}
				
				for(n = 0;n < (event.target).length;n ++)
				{
					if((event.target).text.charCodeAt(n) > 255)
					{
						nLen = nLen + 2;
					}
					else
					{
						nLen ++;
					}
					if(nLen > (event.target).maxChars)
					{
						event.target.text = event.target.text.substr(0,nCount);
					}
					nCount ++;
				}
				
/*				if(nLen > (event.target).maxChars)
				{
					event.target.text = event.target.text.substr(0,(event.target).maxChars);
					Alert.show("您输入的字符串超过了限制!","提示信息",4,event.target.parent,my_func);
					event.target.parent.strAlertCode = event.target.id;
				}
*/			}
			
			
		//返回年月日(YYYYMMDD)
		public function returnYearMonthDate(time:String):String{
			return time.substr(0,4).concat(time.substr(5,2)).concat(time.substr(8,2));
		}
		//返回年月日时分秒 (YYYYMMDDHHMISS)
		public function returnYearMonthDateHMS(startTime:String):String{
			return startTime.substr(0,4).concat(startTime.substr(5,2)).concat(startTime.substr(8,2)).concat(startTime.substr(11,2)).concat(startTime.substr(14,2)).concat(startTime.substr(17,2));
		}
		//比较开始时间是否大于结束时间(YYYYMMDD)，如果大于返回ture(比较起始时间和结束时间)
		public function compareDateTime(startTime:String,endTime:String):Boolean{
			var bool:Boolean = false;
			if(startTime.substr(0,4).concat(startTime.substr(5,2)).concat(startTime.substr(8,2))>endTime.substr(0,4).concat(endTime.substr(5,2)).concat(endTime.substr(8,2))){
				bool = true;
			}
			return bool;
		}
		
			//比较开始时间是否大于结束时间(YYYYMMDDHHMISS)，如果大于返回ture(比较起始时间和结束时间)20090101
		public function compareDateTimeHMS(startTime:String,endTime:String):Boolean{
			var bool:Boolean = false;
			if(startTime.substr(0,4).concat(startTime.substr(5,2)).concat(startTime.substr(8,2)).concat(startTime.substr(11,2)).concat(startTime.substr(14,2)).concat(startTime.substr(17,2))>endTime.substr(0,4).concat(endTime.substr(5,2)).concat(endTime.substr(8,2)).concat(endTime.substr(11,2)).concat(endTime.substr(14,2)).concat(endTime.substr(17,2))){
				bool = true;
			}
			return bool;
		}
	}
}