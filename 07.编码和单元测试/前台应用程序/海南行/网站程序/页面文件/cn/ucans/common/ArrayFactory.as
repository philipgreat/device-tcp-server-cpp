package cn.ucans.common
{
	import mx.collections.ArrayCollection;
	import mx.controls.List;
	
	public class ArrayFactory extends ArrayCollection
	{	
		public static var ASC:int = 0;
		public static var DESC:int = 1;
		
		public static var ALL_ITEM:int = 0;
		public static var NULL_ITEM:int = 1;
		
		public static var CharParenthesis:int = 0;
		public static var NumberParenthesis:int = 1;
		
		private var object:Object = null;
		//public static var LoginUserID:String;
		
		public function ArrayFactory(){
		}
		
		public static function getValueFromArray(array:Array,key:String):String{
			var label:String = "";
			for(var index:int=0;index<array.length;index++){
				if(array[index].data.toString() == key){
					return label = array[index].label;
					//break;
				}
			}
			return label;
		}
			
		public static function getValueFroamArrayCollection(array:ArrayCollection,key:String):String{
			var label:String = null;
			for(var index:int=0;index<array.length;index++){
				if(array[index].data.toString() == key){
					label = array[index].label;
					break;
				}
			}
			if(label == null){
				label = key;
			}
			return label;
		}
		
		public static function getComboBoxSelectedItemFromArrayCollection(provider:ArrayCollection,data:String):Object{
			var object:Object = null;
			for(var index:int=0;index<provider.length;index++){
				if(provider[index].data.toString() == data){
					return object = provider[index];
				}
			}
			if(object == null){
				return provider[0];
			}else{
				return object;
			}
		}
		
		public static function getComboBoxSelectedItemFromArray(provider:Array,data:String):Object{
			var object:Object = null;
			for(var index:int=0;index<provider.length;index++){
				if(provider[index].data.toString() == data){
					return object = provider[index];
				}
				
			}
			if(object == null){
				return provider[0];
			}else{
				return object;
			}
		}
		
		public static function getComboBoxSelectedItemFromXMLList(provider:XMLList,data:String):Object{
			var object:Object = null;
			for(var index:int=0;index<provider.length();index++){
				if(provider.item[index].data.toString() == data){
					return object = provider[index];
				}
				
			}
			if(object == null){
				return provider[0];
			}else{
				return object;
			}
		}
		
		public static function splicing(formatCollection:ArrayCollection,addArrayCollection:ArrayCollection,type:int):ArrayCollection{			
			if(type == ArrayFactory.ASC){
				for(var index1:int=0;index1<addArrayCollection.length;index1++){
					formatCollection.addItem(addArrayCollection.getItemAt(index1));
				}
			}else if(type == ArrayFactory.DESC){
				for(var index2:int=0;index2<addArrayCollection.length;index2++){
					formatCollection.addItemAt(addArrayCollection.getItemAt(index2),0);
				}
			}else{
				throw new Error("no supported splicing type without ASC and DESC, unexpected value: "+type.toString());
			}			
			return formatCollection;
		}
		
		public function subList(startIndex:Number,endIndex:int):ArrayFactory{
			var list:ArrayFactory = new ArrayFactory();
			for(var index:int = startIndex;index<endIndex;index++){
				list.addItem(this.getItemAt(index));
			}
			return list;
		}
		
		public function removeList(startIndex:Number,endIndex:int):ArrayFactory{
			var list:ArrayFactory = new ArrayFactory();
			for(var index:int = startIndex;index<endIndex;index++){
				list.removeItemAt(index);
			}
			return list;
		}
		
		public function replaceList(addArrayCollection:ArrayCollection,startIndex:int,endIndex:int):ArrayFactory{
			var length:int = endIndex - startIndex + 1;
			if(length != addArrayCollection.length){
				throw new Error("format list's length "+addArrayCollection.length+" is not match with the length set endIndex and startIndex "+length);
			}else{
				for(var index:int = startIndex;index<endIndex;index++){
					this.setItemAt(addArrayCollection.getItemAt(index - startIndex),index);
				}
			}
			return this;
		}
		
		public static function getStringFromMXList(list:List):String{
			var array:Array = list.selectedItems;
			var value:String = "";
			for(var i:int=0;i<array.length;i++){
				if(i==array.length-1){
					value += array[i].data;
				}else{
					value += array[i].data + ",";
				}
			}
			return value;
		}
		
		public static function getStringFromMXListForSQL(list:List,Type:int):String{
			var array:Array = list.selectedItems;
			var value:String = "";
			if(Type == ArrayFactory.CharParenthesis){
				for(var index1:int=0;index1<array.length;index1++){
					if(index1==array.length-1){
						value += "'"+array[index1].data+"'";
					}else{
						value += "'"+array[index1].data + "',";
					}
				}
			}else if(Type == ArrayFactory.NumberParenthesis){
				for(var index2:int=0;index2<array.length;index2++){
					if(index2==array.length-1){
						value += array[index2].data;
					}else{
						value += array[index2].data + ",";
					}
				}
			}
			return value;
		}
		
		public static function getStringFromArray(array:Array):String{
			var value:String = "";
			for(var i:int=0;i<array.length;i++){
				if(i==array.length-1){
					value += array[i].data;
				}else{
					value += array[i].data + ",";
				}
			}
			return value;
		}
		
		public static function getStringFromArrayForSQL(array:Array,Type:int):String{
			var value:String = "";
			if(Type == ArrayFactory.CharParenthesis){
				for(var index1:int=0;index1<array.length;index1++){
					if(index1==array.length-1){
						value += "'"+array[index1].data+"'";
					}else{
						value += "'"+array[index1].data + "',";
					}
				}
			}else if(Type == ArrayFactory.NumberParenthesis){
				for(var index2:int=0;index2<array.length;index2++){
					if(index2==array.length-1){
						value += array[index2].data;
					}else{
						value += array[index2].data + ",";
					}
				}
			}
			return value;
		}
		
		public static function getStringFromArrayCollection(arrayCollection:ArrayCollection):String{
			var value:String = "";
			for(var i:int=0;i<arrayCollection.length;i++){
				if(i==arrayCollection.length-1){
					value += arrayCollection[i].data;
				}else{
					value += arrayCollection[i].data + ",";
				}
			}
			return value;
		}
		
		public static function getStringFromArrayCollectionForSQL(arrayCollection:ArrayCollection,Type:int):String{
			var value:String = "";
			if(Type == ArrayFactory.CharParenthesis){
				for(var index1:int=0;index1<arrayCollection.length;index1++){
					if(index1==arrayCollection.length-1){
						value += "'"+arrayCollection[index1].data+"'";
					}else{
						value += "'"+arrayCollection[index1].data + "',";
					}
				}
			}else if(Type == ArrayFactory.NumberParenthesis){
				for(var index2:int=0;index2<arrayCollection.length;index2++){
					if(index2==arrayCollection.length-1){
						value += arrayCollection[index2].data;
					}else{
						value += arrayCollection[index2].data + ",";
					}
				}
			}
			return value;
		}
		
		public static function getListSelectedItemFromArrayCollection(arrayCollection:ArrayCollection,key:String,type:int):Array{
			var array:Array = new Array();
			for(var i:int=0;i<arrayCollection.length;i++){
				var item:Object = arrayCollection.getItemAt(i);
				if(item.data == key){
					array.push(item);
					break;
				}				
			}
			if(array.length < 1){
				if(type == ArrayFactory.ALL_ITEM){
					for(var k:int=0;k<arrayCollection.length;k++){
						var elem:Object = arrayCollection.getItemAt(k);
						array.push(elem);
					}						
				}
			}			
			return array;		
		}
		
		public static function getListSelectedIndexArrayCollection(arrayCollection:ArrayCollection,key:String,type:int):Array{
			var array:Array = new Array();
			for(var i:int=0;i<arrayCollection.length;i++){
				var item:Object = arrayCollection.getItemAt(i);
				if(item.data == key){
					array.push(item);
					break;
				}
				break;
			}
			if(array.length < 1){
				if(type == ArrayFactory.ALL_ITEM){
					for(var k:int=0;k<arrayCollection.length;k++){
						var elem:Object = arrayCollection.getItemAt(k);
						array.push(elem);
					}						
				}
			}			
			return array;		
		}
		
		public static function getListSelectedItemFromArray(array_input:Array,key:String,type:int):Array{
			var array:Array = new Array();			
			for(var i:int=0;i<array_input.length;i++){
				var item:Object = array_input[i];
				if(item.data == key){
					array.push(item);
					break;
				}
				break;
			}			
			if(array.length < 1){
				if(type == ArrayFactory.ALL_ITEM){
					for(var k:int=0;k<array_input.length;k++){
						var elem:Object = array_input[k];
						array.push(elem);
					}						
				}
			}			
			return array;			
		}
		
		public static function getArrayFromArrayCollection(arrayCollection:ArrayCollection):Array{
			var array:Array = new Array();
			for(var i:int=0;i<arrayCollection.length;i++){
				array.push(arrayCollection.getItemAt(i));			
			}		
			return array;		
		}
	}
}