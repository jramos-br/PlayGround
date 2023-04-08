async function dataBase(url) {
  let response = await fetch(url);
  let data = await response.text();
  return data;
}

// Inserção de dados no html para select e list (precisa refatorar)
function insertElementsTag(element, paramts) {
  paramts.idCampo.forEach(ele => {
    if(element[ele.position] != '' && element[ele.position] != undefined) {
      let insertElement = document.getElementById(ele.ID);
      if(insertElement) {
        insertElement.innerHTML += `<option value="${element[ele.position]}">${element[ele.position]}</options>`;
      }
    }
  });
}

function captureInfo(url) {
  GetSpringCM('https://apidownloadeu11.springcm.com/v201411/documents/b2331b1f-1a3d-ea11-9c33-30e17155e337')
  .then(
    data => {
      console.log(data);
    }
  );
}

// prepara dados e chama function para inserção no html
function prepareData(data, paramt) {
  let dataSplit = data.split(/\r?\n|\r/);
  dataSplit.forEach(elements => {
    let element = elements.split(',');                  
    insertElementsTag(element, paramt);
  })
}

// chama função de requisição assincrona e prepara infos (precisa refatorar)
function campoBase(paramts) {
  paramts.forEach(paramt => {
    dataBase(paramt.url).then(
      data => {
        prepareData(data, paramt);
      }
    )
  })
}
  
  // regra de campo, coloca campo com hidden ou não de acordo com parametro passado
  function condicionalCampo(paramts) {
    paramts.forEach(paramt => {
      let event = document.getElementById(paramt.elementEvent);
      event.onclick = () => {
        paramt.conditional.forEach(cond => {
          let elementCondicionado = document.getElementById(cond.elementCondicionado);
          if(event.value == cond.valueCompare) {
            elementCondicionado.hidden = false;
          }else{
            elementCondicionado.hidden = true;
          }
        })
      }
    })
  }
  
  // regra de campo, altera para apenas leitura ou não e obrigatório ou não (precisa refatorar, mas funcional)
  function requiredReadonly(paramts) {
    paramts.forEach(paramt => {
      let event = [];
      event.push(document.getElementById(paramt.elementEvent));
      event.forEach(ev => {
        ev.onclick = () => {
          paramt.elementCondicionado.forEach(id => {
            let element = document.getElementById(id.ID);
            if(ev.value == id.valueCompare || ev.checked == id.valueCompare) {
              if(paramt.required == true) {
                element.setAttribute('required', 'required');
              }
              if(paramt.readonly == true) {
                element.removeAttribute('readonly');
                element.removeAttribute('disabled');
              }
            }else{
              if(paramt.required == true) {
                element.removeAttribute('required');
              }
              if(paramt.readonly == true) {
                element.setAttribute('readonly', 'readonly');
                element.setAttribute('disabled', 'disabled');
              }
            }
          })
        }
        
      })
    })
  }
  
  //trata token e salva no session storage 
  function token() {
    dataBase("http://177.44.44.121:8080/api/token").then(
    data => {  
      let token = JSON.parse(data);
      let time = token.expires_in * 1000;
      window.sessionStorage.setItem('userToken', token.access_token);
      window.sessionStorage.setItem('tokenType', token.token_type);
      window.sessionStorage.setItem('tokenExpires', time);
      console.log(window.sessionStorage.getItem('userToken'));
    }
    ).catch( (err) => {
      console.error(err);
    });
  } 
  
  //regra de faixas
  function addFaixas() {
    return;
  }
  
  function removeFaixas() {
    return;
  }
  
   // requisicao GET no springCM (em testes)
  function GetSpringCM(url) {
    fetch(url, {
      mode: 'cors',
      method: 'GET',
      headers: new Headers ({
        // 'Access-Control-Allow-Origin':'*',
        'Accept' : '*/*',
        'Host': 'apieu11.springcm.com',
        'path': '/v201411/accounts/current',
        'Authorization' : `${window.sessionStorage.getItem('tokenType')} ${window.sessionStorage.getItem('userToken')}`
      })
    })
    .then(response => response.json())
    .then(data => { 
      return data;
    })
    .catch(error => console.error(error));
  }

  async function PostSpringCM() {
    let response = await fetch(
      'https://apieu11.springcm.com/v201411/workflows', {
        mode: 'cors',
        method: 'POST',
        headers: new Headers ({
          'Content-Type': 'application/json',
          'Accept':' application/json',
          'Host': 'apieu11.springcm.com',
          'path': '/v201411/accounts/current',
          'Authorization' : `${window.sessionStorage.getItem('tokenType')} ${window.sessionStorage.getItem('userToken')}`
        }),
        body: JSON.stringify({
          "Name":"CHARLYSTON",
          "Params": "<Params><Davi>Davi</Davi><Rodrigues>Rodrigues</Rodrigues></Params>"
        })
    })
    let data = await response.json();
    console.log(data);
  }

  token();
   
  export {
    campoBase,
    condicionalCampo,
    requiredReadonly,
    token,
    GetSpringCM,
    PostSpringCM,
    captureInfo
  };
  