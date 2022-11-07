let tabs;
let inputs;
let acc;

const valErrClass = 'validation-error';
const hostname = '';

function save() {
    try {
        const res = confirm('Are you sure you want to apply changes?');
        if (res) {
            const credentials = getCredentials();
            const params = `mail=${credentials.mail}&token=${credentials.token}&hostname=${credentials.hostname}&brokerPort=${credentials.brokerport}&product_id=${credentials.productid}&device_id=${credentials.deviceid}`;
            const xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function () {
                if (xhttp.readyState == 4) {
                    if (xhttp.status == 200) {

                    } else if (xhttp.status == 0) {
                        alert('Server closed the connection abruptly!');
                    } else {
                        alert(xhttp.status + ' Error!\n' + xhttp.responseText);
                    }
                }
            };
            xhttp.open('GET', `/setcredentials?${params}`, true);
            xhttp.send();
        }
    } catch (e) {
        console.error(e);
        alert(e.message);
    }
}

function reboot() {
    const xmlhttp = new XMLHttpRequest();
    xmlhttp.open("GET", `/reboot`, true);
    xmlhttp.onreadystatechange = function () {
        if (xmlhttp.readyState == 4) {
            if (xmlhttp.status == 200) alert(xmlhttp.responseText);
        }
    };
    xmlhttp.send(null);
}

function getCredentials() {
    return {
        mail: inputs.mail.value,
        token: inputs.token.value,
        hostname: inputs.hostname.value,
        brokerport: inputs.brokerport.value,
        productid: inputs.product.value,
        deviceid: inputs.device.value
    }
}

function locationDetector() {
    const pathname = window.location.pathname.replace(/^\//, '') || 'index.html';
    const header = document.getElementById('header');
    const el = header.querySelectorAll(`a[href$="${pathname}"]`);

    if (el.length) el[0].classList.add('bold');
}

window.onload = () => {
    acc = document.getElementsByClassName('accordion');

    document.getElementById('btn_save').onclick = save;

    inputs = {
        mail: document.getElementById('mailid'),
        token: document.getElementById('tokenid'),
        hostname: document.getElementById('hostnameid'),
        brokerport: document.getElementById('brokerportid'),
        product: document.getElementById('productid'),
        device: document.getElementById('deviceid')
    }

    for (let i = 0; i < acc.length; i++) {
        acc[i].addEventListener('click', function () {
            this.classList.toggle('active');

            const panel = this.nextElementSibling;

            if (panel.style.maxHeight) {
                panel.style.maxHeight = null;
            } else {
                panel.style.maxHeight = `${panel.scrollHeight}px`;
            }
            return false;
        });
    }

    locationDetector();
}
