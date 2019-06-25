var currentUser;
var table;
function login(event){
    event.preventDefault();
    var username = document.getElementById("usernameInput").value;
    var password = document.getElementById("passwordInput").value;
    tasker.login(username, password, function(user, error){
        if(error){
            alert(error);
        }else{
            currentUser = user;
            document.getElementById("logout").textContent = "Welcome " + username;
            document.getElementById("home").className = "on";
            document.getElementById("login").className = "off";
            table = document.getElementById("tasks");
            makeTable(table);
        }
    });
    return false;
}

function remakeTable(table){
    while(table.hasChildNodes()){
        table.removeChild(table.firstChild);
    }
    makeTable(table);
}

function makeTable(table){
    var row = document.createElement("tr");
    var headers = ["Description", "Color", "Due", "Completed"];
    headers.forEach(function(elem){
        var th = document.createElement("th");
        th.appendChild(document.createTextNode(elem));
        row.appendChild(th);
    });
    table.appendChild(row);
    tasker.tasks(currentUser.id, 
        function(tasks, error){
            if(error){ 
                alert(error);
            }else{
                tasks.forEach(function(task){ makeTask(table, task); });
            }
        });
}

function makeTask(table, task){
    var row = document.createElement("tr");
    row.setAttribute("id", task.id);
    row.className = "on";
    //DESCRIPTION
    var td1 = document.createElement("td");
    td1.appendChild(document.createTextNode(task.desc));
    //COLOR
    var td2 = document.createElement("td");
    var color = document.createElement("input");
    color.setAttribute("type", "color");
    color.setAttribute("value", task.color);
    td2.appendChild(color);
    //DUE DATE
    var td3 = document.createElement("td");
    var today = new Date();
    var circ = document.createElement("span");
    circ.textContent = "!";
    if(isOverdue(task)){
        circ.className = "on";
    }else{
        circ.className = "off";
    }
    td3.appendChild(document.createTextNode(task.due.toLocaleDateString()));
    //COMPLETE CHECK
    var td4 = document.createElement("td");
    var check = document.createElement("input");
    check.setAttribute("type", "checkbox");
    if(task.complete){
        check.checked = true;
    }else{
        check.checked = false;
    }
    td4.appendChild(check);
    td4.appendChild(circ);
    check.addEventListener("change", function(){ remakeTable(table); editTask(task, td4); });
    //TRASH
    var trash = document.createElement("button");
    trash.value = "捨";
    trash.addEventListener("click", function(){ deleteTask(task); });
    var td5 = document.createElement("td");
    td5.appendChild(trash);
    row.appendChild(td1);
    row.appendChild(td2);
    row.appendChild(td3);
    row.appendChild(td4);
    row.appendChild(td5);
    table.appendChild(row);
}

function editTask(task){
    var flag = true;
    if(task.complete){
        flag = false;
    }
    var newTask = {"complete": flag};
    tasker.edit(task.ownerId, task.id, newTask, 
        function(task, error){ 
            if(error){
                alert(error);
            }
        });
}

function deleteTask(task){
    var table = document.getElementById("tasks");
    tasker.delete(task.ownerId, task.id,
        function(task, error){
            if(error){
                alert(error);
            }else{
                var vict = document.getElementById(task.id);
                table.removeChild(vict);
            } 
        });
}

function addTask(){
    var table = document.getElementById("tasks");
    var desc = document.getElementById("descField").value;
    var color = document.getElementById("cField").value;
    var due = document.getElementById("dField").value;
    var task = {"desc": desc, "color": color, "due": due};
    tasker.add(currentUser.id, task, 
        function(task, error){
            if(error){
                alert(error);
            }else{
                //makeTask(table, task);
            }
        });
    remakeTable(table);
}

function isOverdue(task){
    return task.due < new Date() && !task.complete;
}

function searchTextChange(){
    overdueChange();
}

function recallText(){
    var search = document.getElementById("searchField");
    tasker.tasks(currentUser.id, 
        function(tasks, error){
            if(error){ 
                alert(error);
            }else{
                tasks.forEach(function(task){
                                    var node = document.getElementById(task.id); 
                                    if(!task.desc.includes(search.value)){       
                                        node.className = "off"; 
                                    }
                                });
            }
        });
}

function incompleteChange(){
    var overdue = document.getElementById("overdueBox");
    var incomp = document.getElementById("incompleteBox");
    if(overdue.checked){
        recallText();
    }else if(incomp.checked){       
        tasker.tasks(currentUser.id, 
            function(tasks, error){
                if(error){
                    alert(error);
                }else{
                    tasks.forEach(function(task){
                                        var node = document.getElementById(task.id);
                                        var childs = node.childNodes;
                                        if(childs[3].firstChild.checked){
                                            node.className = "off";
                                        }else{
                                            node.className = "on";
                                        }
                                });
                }
                recallText();
            });
    }else{
        var king = document.getElementById("tasks");
        var head = king.firstChild;
        while(head.nextSibling){
            head.className = "on";
            head = head.nextSibling;
        }
        recallText();
    }
}

function overdueChange(){
    var overdue = document.getElementById("overdueBox");
    if(overdue.checked){
        tasker.tasks(currentUser.id,
            function(tasks, error){
                if(error){
                    alert(error);
                }else{
                    tasks.forEach(function(task){
                                        var node = document.getElementById(task.id);
                                        if(!isOverdue(task)){
                                            node.className = "off";
                                        }else{
                                            node.className = "on";
                                        }
                                });
                }
                incompleteChange();
            });
    }else{
        incompleteChange(); 
    }
}

function logout(){
    tasker.logout(function(){
                    document.getElementById("home").className = "off";
                    document.getElementById("login").className = "on";
                    document.getElementById("passwordInput").value = "";
                    document.getElementById("usernameInput").value = "";
                });
}
