// ============================================================
//  MiniCAD Geometry Engine — JavaScript Port of C++ Backend
//  All math is ported 1:1 from GeometryMath.cpp & Shapes.cpp
// ============================================================

const state = {
    lines: [],
    circles: []
};

// ---- Logging ----
function logMsg(msg) {
    const list = document.getElementById('logs-list');
    const li = document.createElement('li');
    li.textContent = msg;
    list.appendChild(li);
    list.parentElement.scrollTop = list.parentElement.scrollHeight;
}

// ---- UI Actions ----
function addLine() {
    const x1 = parseFloat(document.getElementById('l-x1').value);
    const y1 = parseFloat(document.getElementById('l-y1').value);
    const x2 = parseFloat(document.getElementById('l-x2').value);
    const y2 = parseFloat(document.getElementById('l-y2').value);
    if (isNaN(x1) || isNaN(y1) || isNaN(x2) || isNaN(y2)) {
        logMsg("Error: Invalid line coordinates.");
        return;
    }
    state.lines.push({ x1, y1, x2, y2 });
    logMsg(`Queued Line: (${x1}, ${y1}) → (${x2}, ${y2})`);
}

function addCircle() {
    const cx = parseFloat(document.getElementById('c-cx').value);
    const cy = parseFloat(document.getElementById('c-cy').value);
    const r  = parseFloat(document.getElementById('c-r').value);
    if (isNaN(cx) || isNaN(cy) || isNaN(r) || r <= 0) {
        logMsg("Error: Invalid circle parameters.");
        return;
    }
    state.circles.push({ cx, cy, r });
    logMsg(`Queued Circle: Center(${cx}, ${cy}), R=${r}`);
}

// ============================================================
//  GEOMETRY MATH — Ported from GeometryMath.cpp (C++)
// ============================================================

// Distance between two points — distance()
function distance(x1, y1, x2, y2) {
    return Math.sqrt((x2 - x1) ** 2 + (y2 - y1) ** 2);
}

// Distance from point (px, py) to line — distanceToLine()
function distanceToLine(px, py, lx1, ly1, lx2, ly2) {
    const num = Math.abs((lx2 - lx1) * (ly1 - py) - (lx1 - px) * (ly2 - ly1));
    const den = distance(lx1, ly1, lx2, ly2);
    return den === 0 ? distance(px, py, lx1, ly1) : num / den;
}

// Line-Line Intersection — intersectLineLine()
// Returns {x, y} or null if parallel
function intersectLineLine(x1, y1, x2, y2, x3, y3, x4, y4) {
    const den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    if (Math.abs(den) < 1e-9) return null; // parallel

    const t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;

    return {
        x: x1 + t * (x2 - x1),
        y: y1 + t * (y2 - y1)
    };
}

// Line-Circle Intersection — intersectLineCircle()
// Returns array of 0, 1, or 2 {x, y} intersection points
function intersectLineCircle(lx1, ly1, lx2, ly2, cx, cy, r) {
    const dx = lx2 - lx1;
    const dy = ly2 - ly1;
    const fx = lx1 - cx;
    const fy = ly1 - cy;

    const a = dx * dx + dy * dy;
    const b = 2 * (fx * dx + fy * dy);
    const c = fx * fx + fy * fy - r * r;

    let discriminant = b * b - 4 * a * c;
    if (discriminant < 0) return [];

    const points = [];
    if (Math.abs(discriminant) < 1e-9) {
        const t = -b / (2 * a);
        points.push({ x: lx1 + t * dx, y: ly1 + t * dy });
    } else {
        const t1 = (-b - Math.sqrt(discriminant)) / (2 * a);
        const t2 = (-b + Math.sqrt(discriminant)) / (2 * a);
        points.push({ x: lx1 + t1 * dx, y: ly1 + t1 * dy });
        points.push({ x: lx1 + t2 * dx, y: ly1 + t2 * dy });
    }
    return points;
}

// ============================================================
//  SVG GENERATION — Ported from SVGExporter.cpp & Shapes.cpp
// ============================================================

function generateSVG(lines, circles) {
    const W = 800, H = 600;
    const intersectionPoints = [];

    // Line-Line intersections
    for (let i = 0; i < lines.size; i++) {
        for (let j = i + 1; j < lines.size; j++) {
            const l1 = lines[i], l2 = lines[j];
            const pt = intersectLineLine(l1.x1, l1.y1, l1.x2, l1.y2,
                                         l2.x1, l2.y1, l2.x2, l2.y2);
            if (pt) {
                intersectionPoints.push(pt);
                logMsg(`Line-Line intersection at (${pt.x.toFixed(1)}, ${pt.y.toFixed(1)})`);
            }
        }
    }

    // Line-Circle intersections
    for (const l of lines) {
        for (const c of circles) {
            const pts = intersectLineCircle(l.x1, l.y1, l.x2, l.y2, c.cx, c.cy, c.r);
            for (const pt of pts) {
                intersectionPoints.push(pt);
                logMsg(`Line-Circle intersection at (${pt.x.toFixed(1)}, ${pt.y.toFixed(1)})`);
            }
        }
    }

    // Build SVG markup
    let svg = `<svg xmlns="http://www.w3.org/2000/svg" width="${W}" height="${H}" viewBox="0 0 ${W} ${H}" style="width:100%;height:100%">`;
    svg += `<rect width="100%" height="100%" fill="transparent" stroke="rgba(255,255,255,0.1)" stroke-width="1"/>`;

    // Draw lines
    for (const l of lines) {
        svg += `<line x1="${l.x1}" y1="${l.y1}" x2="${l.x2}" y2="${l.y2}"
            stroke="#60a5fa" stroke-width="2.5" stroke-linecap="round"/>`;

        // Distance label at midpoint
        const mx = (l.x1 + l.x2) / 2, my = (l.y1 + l.y2) / 2;
        const len = distance(l.x1, l.y1, l.x2, l.y2).toFixed(1);
        svg += `<text x="${mx + 6}" y="${my - 6}" fill="#93c5fd" font-size="11" font-family="Inter,sans-serif">${len}px</text>`;
    }

    // Draw circles
    for (const c of circles) {
        svg += `<circle cx="${c.cx}" cy="${c.cy}" r="${c.r}"
            stroke="#34d399" stroke-width="2.5" fill="none"/>`;
        svg += `<text x="${c.cx + 6}" y="${c.cy - c.r - 6}" fill="#6ee7b7" font-size="11" font-family="Inter,sans-serif">r=${c.r}</text>`;
    }

    // Draw intersection markers
    for (const pt of intersectionPoints) {
        svg += `<circle cx="${pt.x}" cy="${pt.y}" r="6" fill="#f43f5e" stroke="#fda4af" stroke-width="1.5"/>`;
        svg += `<text x="${pt.x + 8}" y="${pt.y - 6}" fill="#fda4af" font-size="10" font-family="Inter,sans-serif">(${pt.x.toFixed(0)},${pt.y.toFixed(0)})</text>`;
    }

    svg += `</svg>`;
    return svg;
}

// ============================================================
//  MAIN RENDER — Replaces the fetch() call to C++ server
// ============================================================

function renderGeometry() {
    const dot = document.querySelector('.dot');
    const status = document.getElementById('status');

    if (state.lines.length === 0 && state.circles.length === 0) {
        logMsg("No geometry queued. Add a line or circle first.");
        return;
    }

    dot.classList.add('active');
    status.innerHTML = '<span class="dot active"></span> Computing geometry (C++ Math Engine)...';
    logMsg(`Processing ${state.lines.length} line(s), ${state.circles.length} circle(s)...`);

    // Small timeout to let the UI update before computing
    setTimeout(() => {
        // Fix: correct property access
        const linesArr = state.lines;
        const circlesArr = state.circles;

        // Line-Line intersections
        const intersectionPoints = [];
        for (let i = 0; i < linesArr.length; i++) {
            for (let j = i + 1; j < linesArr.length; j++) {
                const l1 = linesArr[i], l2 = linesArr[j];
                const pt = intersectLineLine(l1.x1, l1.y1, l1.x2, l1.y2,
                                             l2.x1, l2.y1, l2.x2, l2.y2);
                if (pt) {
                    intersectionPoints.push(pt);
                    logMsg(`Line-Line intersection at (${pt.x.toFixed(1)}, ${pt.y.toFixed(1)})`);
                } else {
                    logMsg(`Lines ${i + 1} & ${j + 1} are parallel — no intersection.`);
                }
            }
        }

        // Line-Circle intersections
        for (const l of linesArr) {
            for (const c of circlesArr) {
                const pts = intersectLineCircle(l.x1, l.y1, l.x2, l.y2, c.cx, c.cy, c.r);
                for (const pt of pts) {
                    intersectionPoints.push(pt);
                    logMsg(`Line-Circle intersection at (${pt.x.toFixed(1)}, ${pt.y.toFixed(1)})`);
                }
                if (pts.length === 0) logMsg(`Line does not intersect circle (distance > radius).`);
            }
        }

        // Build SVG
        const W = 800, H = 600;
        let svg = `<svg xmlns="http://www.w3.org/2000/svg" width="${W}" height="${H}" viewBox="0 0 ${W} ${H}" style="width:100%;height:100%">`;
        svg += `<rect width="100%" height="100%" fill="transparent" stroke="rgba(255,255,255,0.1)" stroke-width="1"/>`;

        for (const l of linesArr) {
            svg += `<line x1="${l.x1}" y1="${l.y1}" x2="${l.x2}" y2="${l.y2}" stroke="#60a5fa" stroke-width="2.5" stroke-linecap="round"/>`;
            const mx = (l.x1 + l.x2) / 2, my = (l.y1 + l.y2) / 2;
            const len = distance(l.x1, l.y1, l.x2, l.y2).toFixed(1);
            svg += `<text x="${mx + 6}" y="${my - 6}" fill="#93c5fd" font-size="11" font-family="Inter,sans-serif">${len}px</text>`;
        }

        for (const c of circlesArr) {
            svg += `<circle cx="${c.cx}" cy="${c.cy}" r="${c.r}" stroke="#34d399" stroke-width="2.5" fill="none"/>`;
            svg += `<text x="${c.cx + 6}" y="${c.cy - c.r - 6}" fill="#6ee7b7" font-size="11" font-family="Inter,sans-serif">r=${c.r}</text>`;
        }

        for (const pt of intersectionPoints) {
            svg += `<circle cx="${pt.x}" cy="${pt.y}" r="6" fill="#f43f5e" stroke="#fda4af" stroke-width="1.5"/>`;
            svg += `<text x="${pt.x + 8}" y="${pt.y - 6}" fill="#fda4af" font-size="10" font-family="Inter,sans-serif">(${pt.x.toFixed(0)},${pt.y.toFixed(0)})</text>`;
        }

        svg += `</svg>`;

        document.getElementById('canvas-container').innerHTML = svg;
        logMsg(`✓ Successfully rendered ${linesArr.length + circlesArr.length} shapes with ${intersectionPoints.length} intersection(s).`);

        // Reset state
        state.lines = [];
        state.circles = [];

        dot.classList.remove('active');
        status.innerHTML = '<span class="dot"></span> Backend Idle';
    }, 80);
}
