 var x = [1, 2, 'hello', 6+5];
 print(x);
 var y = add(43, 9);

function initView(){
    draw();
}
// person.setName("JerryZhu");
//    person.setAge(25);
//    print(person.name,person.age);
//    print(person.name,person.age);
//    var per = new Person("JIMI",20);
//    print(per.name,per.age);
function Point(x,y){
     this.x=x;
     this.y=y;
}
Point.prototype.show=function(){
  return '(x,y) = '+this.x+','+this.y;
}
//Point.prototype.time = 1000;