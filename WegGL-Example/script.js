
// Importar Three.js
const scene = new THREE.Scene();
const camera = new THREE.PerspectiveCamera(
  75, window.innerWidth / window.innerHeight, 0.1, 1000
);
const renderer = new THREE.WebGLRenderer({ antialias: true });
renderer.setSize(window.innerWidth, window.innerHeight);
document.body.appendChild(renderer.domElement);

// Estrellas
const starCount = 2000;
const starPositions = new Float32Array(starCount * 3);
for (let i = 0; i < starCount * 3; i++) {
  starPositions[i] = (Math.random() - 0.5) * 100;
}
const starsGeometry = new THREE.BufferGeometry();
starsGeometry.setAttribute('position', new THREE.BufferAttribute(starPositions, 3));
const starsMaterial = new THREE.PointsMaterial({ color: 0xffffff, size: 0.5 });
const starField = new THREE.Points(starsGeometry, starsMaterial);
scene.add(starField);

// Luces
scene.add(new THREE.AmbientLight(0xffffff, 0.3));
const pointLight = new THREE.PointLight(0xffccaa, 1.5);
pointLight.position.set(5, 5, 5);
scene.add(pointLight);

// Variables globales
const meshGroup = new THREE.Group();
let frontMesh, backMesh, edgeLines;
let geometry = new THREE.BoxGeometry(2, 2, 2);

let selectedColor = "#00ffd0";

const frontMaterial = new THREE.MeshStandardMaterial({
  color: selectedColor,
  transparent: true,
  opacity: 1.0,
  side: THREE.FrontSide,
  roughness: 0.2,
  metalness: 0.9
});

const backMaterial = new THREE.MeshBasicMaterial({
  color: selectedColor,
  transparent: true,
  opacity: 0.3,
  side: THREE.BackSide,
  wireframe: true,
  polygonOffset: true,
  polygonOffsetFactor: -1
});

function createFigure(geoType) {
  meshGroup.clear();

  switch (geoType) {
    case "box":
      geometry = new THREE.BoxGeometry(2, 2, 2);
      break;
    case "sphere":
      geometry = new THREE.SphereGeometry(1.5, 32, 32);
      break;
    case "tetrahedron":
      geometry = new THREE.TetrahedronGeometry(2);
      break;
    case "torus":
      geometry = new THREE.TorusGeometry(1.2, 0.5, 16, 100);
      break;
    default:
      geometry = new THREE.BoxGeometry(2, 2, 2);
  }

  frontMesh = new THREE.Mesh(geometry, frontMaterial);
  backMesh = new THREE.Mesh(geometry, backMaterial);
  edgeLines = new THREE.LineSegments(
    new THREE.EdgesGeometry(geometry),
    new THREE.LineBasicMaterial({ color: 0xffffff })
  );

  meshGroup.add(frontMesh);
  meshGroup.add(backMesh);
  meshGroup.add(edgeLines);
}

createFigure("box");
scene.add(meshGroup);

// Cámara
camera.position.z = 1;

// Animación
function animate() {
  requestAnimationFrame(animate);
  meshGroup.rotation.x += 0.01;
  meshGroup.rotation.y += 0.01;
  starField.rotation.y += 0.0005;
  pointLight.position.x = Math.sin(Date.now() * 0.001) * 5;
  pointLight.position.z = Math.cos(Date.now() * 0.001) * 5;
  renderer.render(scene, camera);
}
animate();

// UI
const toggleBtn = document.getElementById("toggleBtn");
const statusLabel = document.getElementById("statusLabel");
const opacitySlider = document.getElementById("opacityRange");
const opacityValue = document.getElementById("opacityValue");
const toast = document.getElementById("toast");
const shapeSelector = document.getElementById("shapeSelector");
const colorPicker = document.getElementById("colorPicker");

let backFaceVisible = false;

toggleBtn.addEventListener("click", () => {
  backFaceVisible = !backFaceVisible;
  backMesh.visible = backFaceVisible;

  toggleBtn.textContent = backFaceVisible
    ? "Activar Back-Face Culling"
    : "Desactivar Back-Face Culling";

  statusLabel.textContent = backFaceVisible
    ? "🧩 Back-Face Culling DESACTIVADO"
    : "🔒 Back-Face Culling ACTIVADO";

  toggleBtn.classList.toggle("deactivated", backFaceVisible);
  statusLabel.style.background = backFaceVisible ? "#800" : "#222";
  statusLabel.style.color = backFaceVisible ? "#ff6666" : "#0f0";

  showToast(backFaceVisible
    ? "Back-Face Culling desactivado"
    : "Back-Face Culling activado");
});

opacitySlider.addEventListener("input", () => {
  const val = parseFloat(opacitySlider.value);
  frontMaterial.opacity = val;
  backMaterial.opacity = val * (backFaceVisible ? 0.7 : 0.3);
  opacityValue.textContent = val.toFixed(1);
});

shapeSelector.addEventListener("change", (e) => {
  createFigure(e.target.value);
  backMesh.visible = backFaceVisible;
});

colorPicker.addEventListener("input", (e) => {
  selectedColor = e.target.value;
  frontMaterial.color.set(selectedColor);
  backMaterial.color.set(selectedColor);
});

document.addEventListener("DOMContentLoaded", () => {
    const toggleMenuBtn = document.getElementById("toggleMenuBtn");
    const controlPanel = document.getElementById("controlPanel");
  
 
    toggleMenuBtn.addEventListener("click", () => {
      controlPanel.classList.toggle("hidden");
      if (controlPanel.classList.contains("hidden")) {
        toggleMenuBtn.textContent = "Mostrar Menú";
      } else {
        toggleMenuBtn.textContent = "Cerrar Menú";
      }
    });
  });
  
  

function showToast(msg) {
  toast.textContent = msg;
  toast.classList.add("show");
  setTimeout(() => toast.classList.remove("show"), 2500);
}

// Inicio: ocultar backface
backMesh.visible = false;

window.addEventListener("resize", () => {
  camera.aspect = window.innerWidth / window.innerHeight;
  camera.updateProjectionMatrix();
  renderer.setSize(window.innerWidth, window.innerHeight);
});
