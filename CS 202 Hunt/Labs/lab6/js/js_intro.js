function parseEmail(addr){
    var email = addr.split('@');
    var contact = {};
    if(!email[0] || !email[1]){
        return undefined;
    }
    contact['username'] = email[0];
    contact['domain'] = email[1];
    return contact;
}

function winners(teams){
    var wins = [];
    for(var i = 0; i < teams.length; i++){
        var game = teams[i];
        var home = game['home'];
        var away = game['away'];
        if(away['score'] > home['score']){
            wins.push(away['name']);
        }else if(away['score'] < home['score']){
            wins.push(home['name']);
        }else {
            wins.push("tie");
        }
    }
    return wins;
} 

function rank(names){
    if(names.length == 0){
        return [];
    }
    lcnames = [];
    for(var i = 0; i < names.length; i++){
        lcnames.push(names[i].toLowerCase());
    }
    var count = function(name, array){
        var k = 0;
        for(var i = 0; i < array.length; i++){
            if(array[i] == name){
                k++;
            }
        }
        return k;
    }
    var comp = function(name1, name2){
        return count(name2, lcnames) - count(name1, lcnames);
    }
    lcnames = lcnames.sort(comp);
    var current = lcnames[0];
    var cands = [current];
    for(var i = 0; i < lcnames.length; i++){
        if(current != lcnames[i] && !cands.includes(lcnames[i])){
            current = lcnames[i];
            cands.push(current);
        }
    }
    return cands;
}




