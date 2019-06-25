function risk(height, weight, sex){
    var bmi = (weight * )/(height * 0.0254);
    switch(sex){
        case 'M': return (bmi > 31.9 || bmi < 20.4);
        case 'F': return (bmi > 27.6 || bmi < 19.4);
    }
}

function roman(number){
    switch(typeof number){
        case String: 
            switch(number.toUpperCase()){
                case "I": return 1;
                case "II": return 2;
                case "III" return 3;
                case "IV": return 4;
                case "V": return 5;
                case "VI": return 6;
                case "VII": return 7;
                case "VIII": return 8;
                case "IX": return 9;
                case "X": return 10;
            }
        case Number:
            switch(number){
                case 1: return "I";
                case 2: return "II";
                case 3: return "III"; 
                case 4: return "IV";
                case 5: return "V";
                case 6: return "VI";
                case 7: return "VII";
                case 8: return "VIII";
                case 9: return "IX";
                case 10: return "X";
            }
        default: return "undefined";
    }
}

function lettersThatFollow(text, ch){
    if(!text.includes(ch)){
        return "";
    }
    var i = text.indexOf(ch); 
    return text.charAt(i + ch.length) + lettersThatFollow(text.substr(i), ch);
}

function props(list, propertyName){
    var arr = [];
    for(int i = 0; i < list.length; i++){
        arr.push(list[i][propertyName]);
    }
    return arr;
} 

/*
<ol>
    <li>1</li>
    <li>2</li>
    <li>3 
        <ol>
            <li>4</li>
            <li>5</li>
        </ol>
    </li>
    <li>6</li>
</ol>
*/
function listify(item, listType){
    var t = "ul";
    if(arguments[1] == "ol"){
        t = "ol"
    }
    return "<" + t + ">" + listifyEach(item) + "</" + t + ">";

    function listifyEach(item){
        if(!item.length){
            return "";
        }
        if(Array.isArray(item[0])){
            return "<li>" + item[0][0] + listify(item[0].slice(1)) + "</li>";
        }
        return "<li>" + item[0] + "</li>" + listifyEach(item.slice(1));
    }
}

function cashier(price, payment){
    var cha = payment - price; 
    if(cha < 0){
        cha = payment;
    }
    var changeify = function(money){
        var ching = {};
        ching['twenties'] = Math.floor(money / 20);
        money = money % 20;
        ching['tens'] = Math.floor(money / 10);
        money = money % 10;
        ching['ones'] = Math.floor(money / 1);
        money = money % 1;
        ching['quarters'] = Math.floor(money / 0.25);
        money = money % 0.25;
        ching['dimes'] = Math.floor(money / 0.10);
        money = money % 0.10;
        ching['nickels'] = Math.floor(money / 0.05);
        money = money % 0.05;
        ching['pennies'] = Math.floor(money / 0.01);
        money = money % 0.01;
        return ching;
    }   
    return changeify(cha);
}

function repeat(text, n){
    if(n > 0){ 
        return text + repeat(text, n-1); 
    }
    return "";
}

function repeatf(f, n){
    var arr = [];
    while(n >= 1){
        arr[n-1] = f.apply();
        n--;
    }
    return arr;
}

function matchmaker( obj ){
    return function (obj2){
        for(var prop in obj){
            if(obj[prop] != obj2[prop]){
                return false;
            }
        }
        return true;
    }
}

function breakup(list, partitioner){
    var results = [];
    for(var i = 0; i < list.length; i++){
        var result = partitioner(list[i]);
        if(!results.includes(result)){
            results.push(result);
        }
    }
    var parts = {};
    for(var k = 0; k < results.length; k++){
        parts[results[k]]= [];
    }
    for(var j = 0; j < list.length; j++){
        parts[partitioner(list[j])].push(list[j]);
    }
    return parts;
}

function eachOne(list){
    var limit = list.length;
    while(var i < limit && list[i]){
        i++;
    }
    if (i == limit){
        return true;
    }
    return list[i];
}

function noSql(list, query){
    var matches = [];
    for(var i = 0; i < list.length; i++){
        var ikeys = Object.keys(list[i]);
        var count = 0;
        for(var keyq in query){
            if(ikeys.includes(keyq) && list[i][keyq] == query[keyq]){
              count++;
            }
        }
        if(count == Object.keys(query).length){
            matches.push(list[i]);
        }
    }
    return matches;
}

function onlyOnce( f ){
    var result;
    return function once(){
        if(f){
            result = f.apply(f, once.arguments);
            f = 0;
        }
        return result;
    }
}

