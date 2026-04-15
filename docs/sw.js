const CACHE_NAME = "chaos-groove-pwa-v1";

const FILES_TO_CACHE = [
  "./",
  "./index.html",
  "./index.js",
  "./index.wasm",
  "./index.data",
  "./manifest.json"
];

self.addEventListener("install", (e) => {
  e.waitUntil(
    caches.open(CACHE_NAME).then(cache => cache.addAll(FILES_TO_CACHE))
  );
});

self.addEventListener("fetch", (e) => {
  e.respondWith(
    caches.match(e.request).then(response => response || fetch(e.request))
  );
});