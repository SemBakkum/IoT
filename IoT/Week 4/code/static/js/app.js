var openMenu = (function(){
    var menuBtn = document.getElementById('menuBtn1');
    var menuBtnArrow = document.getElementById('menuBtnArrow');
    var content = document.getElementById('content');
    
    menuBtn.addEventListener('click', function(e) {
    	e.preventDefault();
        content.classList.toggle('isOpen');
        menuBtnArrow.classList.toggle('menuBtnRotate');
    });
    
}());

openMenu();