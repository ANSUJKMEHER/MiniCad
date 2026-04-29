let geometryState = {
    lines: [],
    circles: []
};

function logMsg(msg) {
    const list = document.getElementById('logs-list');
    const li = document.createElement('li');
    li.textContent = msg;
    list.appendChild(li);
    list.parentElement.scrollTop = list.parentElement.scrollHeight;
}

function addLine() {
    const x1 = parseFloat(document.getElementById('l-x1').value);
    const y1 = parseFloat(document.getElementById('l-y1').value);
    const x2 = parseFloat(document.getElementById('l-x2').value);
    const y2 = parseFloat(document.getElementById('l-y2').value);
    
    geometryState.lines.push({x1, y1, x2, y2});
    logMsg(`Queued Line: (${x1}, ${y1}) to (${x2}, ${y2})`);
}

function addCircle() {
    const cx = parseFloat(document.getElementById('c-cx').value);
    const cy = parseFloat(document.getElementById('c-cy').value);
    const r = parseFloat(document.getElementById('c-r').value);
    
    geometryState.circles.push({cx, cy, r});
    logMsg(`Queued Circle: Center(${cx}, ${cy}), R=${r}`);
}

async function renderGeometry() {
    const dot = document.querySelector('.dot');
    const status = document.getElementById('status');
    
    dot.classList.add('active');
    status.innerHTML = '<span class="dot active"></span> Communicating with C++ Backend...';
    logMsg("Sending geometry payload to backend...");

    try {
        // We will host the C++ backend on port 8080 locally
        const response = await fetch('http://localhost:8080/api/geometry', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(geometryState)
        });

        const data = await response.json();
        
        if(data.status === 'success') {
            document.getElementById('canvas-container').innerHTML = data.svg;
            logMsg("Successfully rendered SVG from C++ engine!");
            
            // Clear state after render for demo simplicity
            geometryState = { lines: [], circles: [] };
        } else {
            logMsg("Backend error: " + data.message);
        }
    } catch (error) {
        logMsg("Failed to connect. Is the C++ server running on port 8080?");
        console.error(error);
    } finally {
        setTimeout(() => {
            dot.classList.remove('active');
            status.innerHTML = '<span class="dot"></span> Backend Idle';
        }, 1000);
    }
}
