static const char PROGMEM INDEX_HTML[] = R"rawliteral(
<!doctype html>
<html>
<head>
        <meta charset="utf-8">
        <meta name="viewport" content="width=device-width,initial-scale=1">
        <title>Панель управления</title>
<style>
body{
    font-family:Arial,Helvetica,sans-serif;
    background:#ffffff;
    color:#000000;
    font-size:16px
}
h2{
    font-size:18px
}
section.main{
    display:flex
}
#menu,section.main{
    flex-direction:column
}
#menu{
    display:none;
    flex-wrap:nowrap;
    min-width:340px;
    background:#363636;
    padding:8px;
    border-radius:4px;
    margin-top:-10px;
    margin-right:10px
}
#content{
    display:flex;
    flex-wrap:wrap;
    align-items:stretch
}
figure{
    padding:0;
    margin:0;
    -webkit-margin-before:0;
    margin-block-start:0;
    -webkit-margin-after:0;
    margin-block-end:0;
    -webkit-margin-start:0;
    margin-inline-start:0;
    -webkit-margin-end:0;
    margin-inline-end:0
}
figure img{
    display:block;
    width:100%;
    height:auto;
    border-radius:4px;
    margin-top:8px
}
@media (min-width: 800px) and (orientation:landscape){
    #content{
        display:flex;
        flex-wrap:nowrap;
        align-items:stretch
    }
    figure img{
        display:block;
        max-width:100%;
        max-height:calc(100vh - 40px);
        width:auto;
        height:auto;
        margin-left: auto;
        margin-right: auto
    }
    figure{
        padding:0;
        margin:0;
        -webkit-margin-before:0;
        margin-block-start:0;
        -webkit-margin-after:0;
        margin-block-end:0;
        -webkit-margin-start:0;
        margin-inline-start:0;
        -webkit-margin-end:0;
        margin-inline-end:0
    }
}
section#buttons{
    display:flex;
    flex-wrap:nowrap;
    justify-content:space-between;
    margin-left: auto;
    margin-right: auto
}
#nav-toggle{
    cursor:pointer;
    display:block
}
#nav-toggle-cb{
    outline:0;
    opacity:0;
    width:0;
    height:0
}
#nav-toggle-cb:checked+#menu{
    display:flex
}
.input-group{
    display:flex;
    flex-wrap:nowrap;
    line-height:22px;
    margin:5px 0
}
.input-group>label{
    display:inline-block;
    padding-right:10px;
    min-width:47%
}
.input-group input,.input-group select{
    flex-grow:1
}
.range-max,.range-min{
    display:inline-block;
    padding:0 5px
}
button{
    display:block;
    margin:7px;
    padding:0 12px;
    border: 3px solid black;
    ;
    line-height:28px;
    cursor:pointer;
    color:#000000;
    background:#ffffff;
    border-radius:5px;
    font-size:16px;
    outline:0
}
button:hover{
    background:#c7c7c7
}
button:active{
    background:#9e9e9e
}
button.disabled{
    cursor:default;
    background:#a0a0a0
}
input[type=range]{
    -webkit-appearance:none;
    width:100%;
    height:22px;
    background:#ffffff;
    cursor:pointer;
    margin:0
}
input[type=range]:focus{
    outline:0
}
input[type=range]::-webkit-slider-runnable-track{
    width:100%;
    height:2px;
    cursor:pointer;
    background:#EFEFEF;
    border-radius:0;
    border:0 solid #EFEFEF
}
input[type=range]::-webkit-slider-thumb{
    border:1px solid rgba(0,0,30,0);
    height:22px;
    width:22px;
    border-radius:50px;
    background:#000000;
    cursor:pointer;
    -webkit-appearance:none;
    margin-top:-11.5px
}
input[type=range]:focus::-webkit-slider-runnable-track{
    background:#EFEFEF
}
input[type=range]::-moz-range-track{
    width:100%;
    height:2px;
    cursor:pointer;
    background:#EFEFEF;
    border-radius:0;
    border:0 solid #EFEFEF
}
input[type=range]::-moz-range-thumb{
    border:1px solid rgba(0,0,30,0);
    height:22px;
    width:22px;
    border-radius:50px;
    background:#ff3034;
    cursor:pointer
}
input[type=range]::-ms-track{
    width:100%;
    height:2px;
    cursor:pointer;
    background:0 0;
    border-color:transparent;
    color:transparent
}
input[type=range]::-ms-fill-lower{
    background:#EFEFEF;
    border:0 solid #EFEFEF;
    border-radius:0
}
input[type=range]::-ms-fill-upper{
    background:#EFEFEF;
    border:0 solid #EFEFEF;
    border-radius:0
}
input[type=range]::-ms-thumb{
    border:1px solid rgba(0,0,30,0);
    height:22px;
    width:22px;
    border-radius:50px;
    background:#ff3034;
    cursor:pointer;
    height:2px
}
input[type=range]:focus::-ms-fill-lower{
    background:#EFEFEF
}
input[type=range]:focus::-ms-fill-upper{
    background:#363636
}
.switch{
    display:block;
    position:relative;
    line-height:22px;
    font-size:16px;
    height:22px
}
.switch input{
    outline:0;
    opacity:0;
    width:0;
    height:0
}
.slider{
    width:50px;
    height:22px;
    border-radius:22px;
    cursor:pointer;
    background-color:grey
}
.slider,.slider:before{
    display:inline-block;
    transition:.4s
}
.slider:before{
    position:relative;
    content:"";
    border-radius:50%;
    height:16px;
    width:16px;
    left:4px;
    top:3px;
    background-color:#fff
}
input:checked+.slider{
    background-color:#ff3034
}
input:checked+.slider:before{
    -webkit-transform:translateX(26px);
    transform:translateX(26px)
}
select{
    border:1px solid #363636;
    font-size:14px;
    height:22px;
    outline:0;
    border-radius:5px
}
.image-container{
    position:relative;
    min-width:160px
}
.close{
    position:absolute;
    right:5px;
    top:5px;
    background:#ff3034;
    width:16px;
    height:16px;
    border-radius:100px;
    color:#fff;
    text-align:center;
    line-height:18px;
    cursor:pointer
}
.hidden{
    display:none
}
          
</style>
</head>
  <body>
    <figure>
      <div id="stream-container" class="image-container hidden">
        <div class="close" id="close-stream">×</div>
        <img id="stream" src="">
      </div>
    </figure>
    <section class="main">
    <section id="buttons">
      <table>
         <tr>
            <td align="center"><button id="get-still">Фото</button></td>
            </td>
            <td align="center"><button id="restart" onclick="fetch(document.location.origin+'/control?var=car&val=6');">Перезагрузка</button></td>
            <td align="center"><button id="toggle-stream">Вкл.стрим</button></td>
         </tr>
         <tr>
            <td><input type="checkbox" id="nostop" onclick="var noStop=0;if (this.checked) noStop=1;fetch(document.location.origin+'/control?var=nostop&val='+noStop);">Без ост.</td>
            <td align="center"><button id="forward" onclick="fetch(document.location.origin+'/control?var=car&val=1');">Вперёд</button></td>
            <td></td>
         </tr>
         <tr>
            <td align="center"><button id="turnleft" onclick="fetch(document.location.origin+'/control?var=car&val=2');">Налево</button></td>
            <td align="center"><button id="stop" onclick="fetch(document.location.origin+'/control?var=car&val=3');">Стоп</button></td>
            <td align="center"><button id="turnright" onclick="fetch(document.location.origin+'/control?var=car&val=4');">Направо</button></td>
         </tr>
         <tr>
            <td>
            </td>
            <td align="center"><button id="backward" onclick="fetch(document.location.origin+'/control?var=car&val=5');">Назад</button></td>
            <td></td>
         </tr>
         <tr>
            <td>Свет</td>
            <td align="center" colspan="2"><input type="range" id="flash" min="0" max="255" value="0" onchange="try{fetch(document.location.origin+'/control?var=flash&val='+this.value);}catch(e){}"></td>
         </tr>
         <tr>
            <td>Скорость</td>
            <td align="center" colspan="2"><input type="range" id="speed" min="0" max="255" value="255" onchange="try{fetch(document.location.origin+'/control?var=speed&val='+this.value);}catch(e){}"></td>
         </tr>
         <tr>
            <td>Серво</td>
            <td align="center" colspan="2"><input type="range" id="servo" min="-45" max="45" value="0" step="5" onchange="try{fetch(document.location.origin+'/control?var=servo&val='+this.value);}catch(e){}"></td>
         </tr>
         <tr>
            <td>Качество</td>
            <td align="center" colspan="2"><input type="range" id="quality" min="10" max="63" value="10" onchange="try{fetch(document.location.origin+'/control?var=quality&val='+this.value);}catch(e){}"></td>
         </tr>
         <tr>
            <td>Разрешение</td>
            <td align="center" colspan="2"><input type="range" id="framesize" min="0" max="6" value="5" onchange="try{fetch(document.location.origin+'/control?var=framesize&val='+this.value);}catch(e){}"></td>
         </tr>
         <tr>
            <td>Частота кадров</td>
            <td align="center" colspan="2"><input type="range" id="framerate" min="5" max="50" value="15" step="5" onchange="try{fetch(document.location.origin+'/control?var=framerate&val='+this.value);}catch(e){}"></td>
         </tr>
      </table>
    </section>
    </section>
    <script>
document.addEventListener('DOMContentLoaded', function() {
    function b(B) {
        let C;
        switch (B.type) {
            case 'checkbox':
                C = B.checked ? 1 : 0;
                break;
            case 'range':
            case 'select-one':
                C = B.value;
                break;
            case 'button':
            case 'submit':
                C = '1';
                break;
            default:
                return;
        }
        const D = `${c}/control?var=${B.id}&val=${C}`;
        fetch(D).then(E => {
            console.log(`request to ${D} finished, status: ${E.status}`)
        })
    }
    var c = document.location.origin;
    const e = B => {
            B.classList.add('hidden')
        },
        f = B => {
            B.classList.remove('hidden')
        },
        g = B => {
            B.classList.add('disabled'), B.disabled = !0
        },
        h = B => {
            B.classList.remove('disabled'), B.disabled = !1
        },
        i = (B, C, D) => {
            D = !(null != D) || D;
            let E;
            'checkbox' === B.type ? (E = B.checked, C = !!C, B.checked = C) : (E = B.value, B.value = C), D && E !== C ? b(B) : !D && ('aec' === B.id ? C ? e(v) : f(v) : 'agc' === B.id ? C ? (f(t), e(s)) : (e(t), f(s)) : 'awb_gain' === B.id ? C ? f(x) : e(x) : 'face_recognize' === B.id && (C ? h(n) : g(n)))
        };
    document.querySelectorAll('.close').forEach(B => {
        B.onclick = () => {
            e(B.parentNode)
        }
    }), fetch(`${c}/status`).then(function(B) {
        return B.json()
    }).then(function(B) {
        document.querySelectorAll('.default-action').forEach(C => {
            i(C, B[C.id], !1)
        })
    });
    const j = document.getElementById('stream'),
        k = document.getElementById('stream-container'),
        l = document.getElementById('get-still'),
        m = document.getElementById('toggle-stream'),
        n = document.getElementById('face_enroll'),
        o = document.getElementById('close-stream'),
        p = () => {
            window.stop(), m.innerHTML = 'Вкл.стрим'
        },
        q = () => {
            j.src = `${c+':81'}/stream`, f(k), m.innerHTML = 'Выкл.стрим'
        };
    l.onclick = () => {
        p(), j.src = `${c}/capture?_cb=${Date.now()}`, f(k)
    }, o.onclick = () => {
        p(), e(k)
    }, m.onclick = () => {
        const B = 'Выкл.стрим' === m.innerHTML;
        B ? p() : q()
    }, n.onclick = () => {
        b(n)
    }, document.querySelectorAll('.default-action').forEach(B => {
        B.onchange = () => b(B)
    });
    const r = document.getElementById('agc'),
        s = document.getElementById('agc_gain-group'),
        t = document.getElementById('gainceiling-group');
    r.onchange = () => {
        b(r), r.checked ? (f(t), e(s)) : (e(t), f(s))
    };
    const u = document.getElementById('aec'),
        v = document.getElementById('aec_value-group');
    u.onchange = () => {
        b(u), u.checked ? e(v) : f(v)
    };
    const w = document.getElementById('awb_gain'),
        x = document.getElementById('wb_mode-group');
    w.onchange = () => {
        b(w), w.checked ? f(x) : e(x)
    };
    const y = document.getElementById('face_detect'),
        z = document.getElementById('face_recognize'),
        A = document.getElementById('framesize');
    A.onchange = () => {
        b(A), 5 < A.value && (i(y, !1), i(z, !1))
    }, y.onchange = () => {
        return 5 < A.value ? (alert('Перед включением этой функции выберите разрешение CIF или более низкое!'), void i(y, !1)) : void(b(y), !y.checked && (g(n), i(z, !1)))
    }, z.onchange = () => {
        return 5 < A.value ? (alert('Перед включением этой функции выберите разрешение CIF или более низкое!'), void i(z, !1)) : void(b(z), z.checked ? (h(n), i(y, !0)) : g(n))
    }
});
        
document.addEventListener('keypress', function(event) {                    // Поддержка управления с клавиатуры
    switch (event.charCode) {
        case 119:
            fetch(document.location.origin + '/control?var=car&val=1');
            break                                                          // Вперёд - W
        case 115:
            fetch(document.location.origin + '/control?var=car&val=5');
            break                                                          // Назад - S
        case 97:
            fetch(document.location.origin + '/control?var=car&val=2');
            break                                                          // Налево - A
        case 100:
            fetch(document.location.origin + '/control?var=car&val=4');
            break                                                          // Направо - D
        case 112:
            fetch(document.location.origin + '/control?var=car&val=3');
            break                                                          // Стоп - P
        case 108:
            fetch(document.location.origin + '/control?var=flash&val=255');
            break                                                          // Включить свет - L
        case 107:
            fetch(document.location.origin + '/control?var=flash&val=0');
            break                                                          // Включить свет- K
    }
});
        
    </script>
  </body>
</html>
)rawliteral";
