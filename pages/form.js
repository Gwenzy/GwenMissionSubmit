function verifForm(){
    return document.getElementById("name").value !== "" && document.getElementById("mission").value !== "" && document.getElementById("xp").value !== "";

}

function keyPressed(){
    if(verifForm())
        document.getElementById("submit").removeAttribute("disabled");
    else
        document.getElementById("submit").setAttribute("disabled", "true");
}