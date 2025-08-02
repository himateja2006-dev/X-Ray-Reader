// Wait for DOM to be fully loaded
document.addEventListener('DOMContentLoaded', function() {
    console.log("DOM loaded, initializing application...");
    
    // Get all required elements
    const elements = {
        loginForm: document.getElementById("loginForm"),
        signupForm: document.getElementById("signupForm"),
        uploadContainer: document.getElementById("uploadContainer"),
        authButtons: document.getElementById("authButtons"),
        userMenu: document.getElementById("userMenu"),
        welcomeUser: document.getElementById("welcomeUser"),
        loginFormEl: document.getElementById("loginFormEl"),
        signupFormEl: document.getElementById("signupFormEl"),
        loginError: document.getElementById("loginError"),
        signupError: document.getElementById("signupError"),
        uploadSuccess: document.getElementById("uploadSuccess"),
        uploadError: document.getElementById("uploadError"),
        fileInput: document.getElementById("fileInput"),
        imagePreview: document.getElementById("imagePreview"),
        uploadButton: document.getElementById("uploadButton"),
        clearButton: document.getElementById("clearButton"),
        gallery: document.getElementById("gallery"),
        uploadArea: document.getElementById("uploadArea"),
        resultSection: document.getElementById("resultSection"),
        predictionDetails: document.getElementById("predictionDetails"),
        loginBtn: document.querySelector(".login-btn"),
        signupBtn: document.querySelector(".signup-btn"),
        loginLink: document.querySelector(".login-link"),
        signupLink: document.querySelector(".signup-link"),
        logoutBtn: document.querySelector(".logout-btn"),
        homeLink: document.querySelector(".home-link"),
        galleryLink: document.querySelector(".gallery-link")
    };

    // Log which elements were found/not found
    Object.entries(elements).forEach(([key, element]) => {
        if (!element) {
            console.warn(`Element not found: ${key}`);
        }
    });

    // Check if critical elements exist
    const criticalElements = ['uploadButton', 'fileInput', 'uploadArea', 'uploadContainer'];
    const missingElements = criticalElements.filter(id => !elements[id]);
    
    if (missingElements.length > 0) {
        console.error('Critical elements not found:', missingElements);
        return;
    }

    // User data storage (in a real app, this would be server-side)
    const users = JSON.parse(localStorage.getItem("users")) || [];
    let currentUser = JSON.parse(localStorage.getItem("currentUser")) || null;
    let userImages = JSON.parse(localStorage.getItem("userImages")) || {};

    console.log("Current user state:", currentUser ? "Logged in" : "Not logged in");

    // Function to check if user is logged in
    function checkUserSession() {
        if (!currentUser) {
            console.log("No user session found");
            return false;
        }
        return true;
    }

    // Function to handle unauthorized access
    function handleUnauthorized() {
        console.log("Unauthorized access, redirecting to login");
        showLoginForm();
    }

    // Authentication Functions
    async function signup(name, email, password) {
        console.log("Attempting signup for:", email);
        if (!elements.signupError) return false;
        
        try {
            const response = await fetch('/signup', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify({ name, email, password })
            });

            const data = await response.json();
            
            if (!response.ok) {
                showError(elements.signupError, data.error || "Signup failed");
                return false;
            }

            console.log("User created successfully");
            return data.user;
        } catch (error) {
            console.error("Signup error:", error);
            showError(elements.signupError, "Failed to connect to server");
            return false;
        }
    }

    async function login(email, password) {
        console.log("Attempting login for:", email);
        if (!elements.loginError) return false;

        try {
            const response = await fetch('/login', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
                credentials: 'include',  // Include cookies
                body: JSON.stringify({ email, password })
            });

            const data = await response.json();
            
            if (!response.ok) {
                showError(elements.loginError, data.error || "Invalid email or password");
                return false;
            }

            // Set current user
            currentUser = data.user;
            localStorage.setItem("currentUser", JSON.stringify(currentUser));
            console.log("Login successful");
            
            // Refresh gallery data after successful login
            await refreshGalleryData();
            
            return true;
        } catch (error) {
            console.error("Login error:", error);
            showError(elements.loginError, "Failed to connect to server");
            return false;
        }
    }

    function logout() {
        console.log("Logging out user");
        currentUser = null;
        localStorage.removeItem("currentUser");
        showLoginForm();
    }

    // UI Functions
    function showLoginForm() {
        console.log("Showing login form");
        if (elements.loginForm) elements.loginForm.classList.remove("hidden");
        if (elements.signupForm) elements.signupForm.classList.add("hidden");
        if (elements.uploadContainer) elements.uploadContainer.classList.add("hidden");
        if (elements.authButtons) elements.authButtons.classList.remove("hidden");
        if (elements.userMenu) elements.userMenu.classList.add("hidden");
        clearErrors();
    }

    function showSignupForm() {
        console.log("Showing signup form");
        if (elements.loginForm) elements.loginForm.classList.add("hidden");
        if (elements.signupForm) elements.signupForm.classList.remove("hidden");
        if (elements.uploadContainer) elements.uploadContainer.classList.add("hidden");
        if (elements.authButtons) elements.authButtons.classList.remove("hidden");
        if (elements.userMenu) elements.userMenu.classList.add("hidden");
        clearErrors();
    }

    function showUploadContainer() {
        console.log("Showing upload container");
        if (!checkUserSession()) {
            handleUnauthorized();
            return;
        }

        if (elements.loginForm) elements.loginForm.classList.add("hidden");
        if (elements.signupForm) elements.signupForm.classList.add("hidden");
        if (elements.uploadContainer) elements.uploadContainer.classList.remove("hidden");
        if (elements.authButtons) elements.authButtons.classList.add("hidden");
        if (elements.userMenu) elements.userMenu.classList.remove("hidden");
        if (elements.welcomeUser) elements.welcomeUser.textContent = `Welcome, ${currentUser.name}!`;
        loadUserImages();
    }

    function showError(element, message) {
        console.log("Showing error:", message);
        if (!element) return;
        element.textContent = message;
        element.classList.remove("hidden");
    }

    function clearErrors() {
        console.log("Clearing all errors");
        if (elements.loginError) elements.loginError.classList.add("hidden");
        if (elements.signupError) elements.signupError.classList.add("hidden");
        if (elements.uploadError) elements.uploadError.classList.add("hidden");
        if (elements.uploadSuccess) elements.uploadSuccess.classList.add("hidden");
    }

    function loadUserImages() {
        console.log("Loading user images");
        if (!elements.gallery) {
            console.error("Gallery element not found");
            return;
        }
        
        elements.gallery.innerHTML = "";

        if (!currentUser || !userImages[currentUser.id]) {
            console.log("No images found for current user");
            elements.gallery.innerHTML = '<div class="text-center text-gray-500 py-4">No diagnosis history found</div>';
            return;
        }

        console.log("Found images:", userImages[currentUser.id]);

        // Sort images by upload date (newest first)
        const sortedImages = [...userImages[currentUser.id]].sort((a, b) => 
            new Date(b.uploadDate) - new Date(a.uploadDate)
        );

        sortedImages.forEach((image) => {
            console.log("Creating gallery item for image:", image);
            const galleryItem = document.createElement("div");
            galleryItem.className = "gallery-item";

            const img = document.createElement("img");
            img.className = "gallery-img";
            img.src = image.url;
            img.alt = image.name;
            img.onerror = () => {
                console.error("Error loading image:", image.url);
                galleryItem.remove();
                // Remove from localStorage
                userImages[currentUser.id] = userImages[currentUser.id].filter(i => i.id !== image.id);
                localStorage.setItem("userImages", JSON.stringify(userImages));
            };

            const predictionInfo = document.createElement("div");
            predictionInfo.className = "prediction-info";
            
            // Format the date
            const uploadDate = new Date(image.uploadDate);
            const formattedDate = uploadDate.toLocaleDateString('en-US', {
                year: 'numeric',
                month: 'long',
                day: 'numeric',
                hour: '2-digit',
                minute: '2-digit'
            });

            // Determine confidence color
            const confidenceColor = image.confidence >= 80 ? 'text-green-600' : 
                                  image.confidence >= 60 ? 'text-yellow-600' : 'text-red-600';

            predictionInfo.innerHTML = `
                <h3>${image.prediction.charAt(0).toUpperCase() + image.prediction.slice(1).toLowerCase()}</h3>
                <p class="${confidenceColor}">Confidence: ${image.confidence.toFixed(1)}%</p>
                <p class="text-gray-600">Uploaded: ${formattedDate}</p>
            `;

            galleryItem.appendChild(img);
            galleryItem.appendChild(predictionInfo);
            elements.gallery.appendChild(galleryItem);
        });
    }

    async function refreshGalleryData() {
        try {
            console.log("Fetching gallery data...");
            const response = await fetch('http://127.0.0.1:8080/user_diagnosis_history', {
                credentials: 'include'  // Include cookies for session
            });
            
            if (response.status === 401) {
                console.log("Unauthorized access");
                showLoginForm();
                return;
            }
            
            if (!response.ok) {
                throw new Error('Failed to fetch gallery data');
            }
            
            const data = await response.json();
            console.log("Received gallery data:", data);
            
            // Update localStorage with fresh data
            if (currentUser && data.images) {
                userImages[currentUser.id] = data.images.map(image => ({
                    id: image._id,
                    name: image.filename,
                    url: `/uploads/${image.filename}`,
                    prediction: image.prediction,
                    confidence: image.confidence,
                    uploadDate: image.upload_date
                }));
                localStorage.setItem("userImages", JSON.stringify(userImages));
                loadUserImages();
            } else {
                console.log("No images found or no current user");
                if (elements.gallery) {
                    elements.gallery.innerHTML = '<div class="text-center text-gray-500 py-4">No diagnosis history found</div>';
                }
            }
        } catch (error) {
            console.error('Error refreshing gallery data:', error);
            if (error.message.includes('Failed to fetch')) {
                showError(elements.uploadError, "Could not connect to server. Please try again.");
            }
        }
    }

    // Add event listeners
    if (elements.loginFormEl) {
        elements.loginFormEl.addEventListener("submit", async function (e) {
            e.preventDefault();
            console.log("Login form submitted");
            const email = document.getElementById("loginEmail")?.value;
            const password = document.getElementById("loginPassword")?.value;
            if (email && password && await login(email, password)) {
                showUploadContainer();
            }
        });
    }

    if (elements.signupFormEl) {
        elements.signupFormEl.addEventListener("submit", async function (e) {
            e.preventDefault();
            console.log("Signup form submitted");
            const name = document.getElementById("signupName")?.value;
            const email = document.getElementById("signupEmail")?.value;
            const password = document.getElementById("signupPassword")?.value;
            const confirmPassword = document.getElementById("confirmPassword")?.value;

            if (!name || !email || !password || !confirmPassword) {
                console.log("Missing required fields");
                return;
            }

            if (password !== confirmPassword) {
                showError(elements.signupError, "Passwords do not match");
                return;
            }

            const user = await signup(name, email, password);
            if (user) {
                currentUser = user;
                localStorage.setItem("currentUser", JSON.stringify(currentUser));
                showUploadContainer();
            }
        });
    }

    // Add navigation event listeners
    if (elements.loginBtn) elements.loginBtn.addEventListener("click", showLoginForm);
    if (elements.signupBtn) elements.signupBtn.addEventListener("click", showSignupForm);
    if (elements.loginLink) elements.loginLink.addEventListener("click", showLoginForm);
    if (elements.signupLink) elements.signupLink.addEventListener("click", showSignupForm);
    if (elements.logoutBtn) elements.logoutBtn.addEventListener("click", logout);

    if (elements.homeLink) {
        elements.homeLink.addEventListener("click", function (e) {
            e.preventDefault();
            if (currentUser) {
                showUploadContainer();
            } else {
                showLoginForm();
            }
        });
    }

    if (elements.galleryLink) {
        elements.galleryLink.addEventListener("click", function (e) {
            e.preventDefault();
            if (currentUser) {
                showUploadContainer();
            } else {
                showLoginForm();
            }
        });
    }

    // Image Upload Functions
    if (elements.fileInput) {
        elements.fileInput.addEventListener("change", function () {
            console.log("File input changed");
            const file = this.files[0];
            if (!file) return;

            handleFileSelection(file);
        });
    }

    if (elements.uploadArea) {
        elements.uploadArea.addEventListener("dragover", function (e) {
            e.preventDefault();
            this.style.borderColor = "var(--primary-color)";
            this.style.backgroundColor = "rgba(79, 70, 229, 0.05)";
        });

        elements.uploadArea.addEventListener("dragleave", function () {
            this.style.borderColor = "#d1d5db";
            this.style.backgroundColor = "#f9fafb";
        });

        elements.uploadArea.addEventListener("drop", function (e) {
            e.preventDefault();
            this.style.borderColor = "#d1d5db";
            this.style.backgroundColor = "#f9fafb";

            const file = e.dataTransfer.files[0];
            if (!file) return;

            handleFileSelection(file);
        });
    }

    function handleFileSelection(file) {
        // Validate file size (max 5MB)
        if (file.size > 5 * 1024 * 1024) {
            showError(elements.uploadError, "File size exceeds 5MB limit");
            return;
        }

        // Validate file type
        if (!file.type.startsWith("image/")) {
            showError(elements.uploadError, "Please select an image file");
            return;
        }

        clearErrors();

        // Update the file input
        const dataTransfer = new DataTransfer();
        dataTransfer.items.add(file);
        if (elements.fileInput) {
            elements.fileInput.files = dataTransfer.files;
        }

        // Show image preview and result section
        const reader = new FileReader();
        reader.onload = function (e) {
            if (elements.imagePreview) {
                elements.imagePreview.src = e.target.result;
                elements.imagePreview.style.display = "block";
            }
            if (elements.resultSection) {
                elements.resultSection.classList.remove("hidden");
            }
            if (elements.uploadArea) {
                elements.uploadArea.style.display = "none";
            }
        };
        reader.readAsDataURL(file);
    }

    if (elements.clearButton) {
        elements.clearButton.addEventListener("click", function() {
            if (elements.fileInput) elements.fileInput.value = "";
            if (elements.imagePreview) {
                elements.imagePreview.style.display = "none";
                elements.imagePreview.src = "";
            }
            if (elements.resultSection) {
                elements.resultSection.classList.add("hidden");
            }
            if (elements.uploadArea) {
                elements.uploadArea.style.display = "flex";
            }
            if (elements.predictionDetails) {
                elements.predictionDetails.innerHTML = "";
            }
            clearErrors();
        });
    }

    if (elements.uploadButton) {
        elements.uploadButton.addEventListener("click", async function () {
            console.log("Upload button clicked");
            if (!checkUserSession()) {
                handleUnauthorized();
                return;
            }

            if (!elements.fileInput?.files[0]) {
                showError(elements.uploadError, "Please select an image to upload");
                return;
            }

            clearErrors();
            console.log("Starting file upload process...");

            const file = elements.fileInput.files[0];
            console.log("Selected file:", file.name, "Size:", file.size, "Type:", file.type);

            const formData = new FormData();
            formData.append('file', file);

            try {
                // Show loading state
                elements.uploadButton.disabled = true;
                elements.uploadButton.textContent = "Analyzing...";
                console.log("Sending request to /predict endpoint...");

                const response = await fetch('http://127.0.0.1:8080/predict', {
                    method: 'POST',
                    body: formData,
                    credentials: 'include',  // Include cookies for session
                    headers: {
                        'Accept': 'application/json',
                    }
                });

                console.log("Response status:", response.status);
                
                if (!response.ok) {
                    const errorData = await response.json();
                    throw new Error(errorData.error || 'Upload failed');
                }

                const result = await response.json();
                console.log("Response data:", result);

                // Update prediction details
                if (elements.predictionDetails) {
                    const predictionText = result.prediction.charAt(0).toUpperCase() + result.prediction.slice(1).toLowerCase();
                    const confidenceColor = result.confidence >= 80 ? 'text-green-600' : 
                                          result.confidence >= 60 ? 'text-yellow-600' : 'text-red-600';
                    
                    elements.predictionDetails.innerHTML = `
                        <div class="prediction-result">
                            <div class="prediction-details">
                                <p class="mb-1"><strong>Condition:</strong> ${predictionText}</p>
                                <p class="${confidenceColor}"><strong>Confidence:</strong> ${result.confidence.toFixed(1)}%</p>
                            </div>
                            ${result.confidence < 60 ? 
                                '<p class="text-sm text-gray-600 mt-2">Note: Low confidence prediction. Please consult a healthcare professional.</p>' : 
                                ''}
                        </div>
                    `;
                }

                // Refresh gallery data from server
                await refreshGalleryData();
                console.log("Gallery updated");

            } catch (error) {
                console.error("Upload error:", error);
                let errorMessage = "Failed to upload image: ";
                if (error.message.includes("Failed to fetch")) {
                    errorMessage += "Could not connect to the server. Please make sure the server is running.";
                } else {
                    errorMessage += error.message;
                }
                showError(elements.uploadError, errorMessage);
            } finally {
                // Reset button state
                if (elements.uploadButton) {
                    elements.uploadButton.disabled = false;
                    elements.uploadButton.textContent = "Analyze Image";
                }
            }
        });
    }

    // Check if user is logged in on page load
    if (currentUser) {
        showUploadContainer();
    } else {
        showLoginForm();
    }
});
