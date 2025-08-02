from flask import Flask, render_template, request, jsonify, session, send_from_directory
from flask_cors import CORS
import tensorflow as tf
import tensorflow_hub as hub
import numpy as np
import os
from PIL import Image
from tensorflow.keras.applications.mobilenet_v2 import MobileNetV2, preprocess_input
from werkzeug.security import generate_password_hash, check_password_hash
from werkzeug.utils import secure_filename
import time
from flask_pymongo import PyMongo
from bson.objectid import ObjectId
from datetime import datetime


app = Flask(__name__, 
    static_folder='static',
    template_folder='template'
)

# MongoDB Configuration
app.config["MONGO_URI"] = "mongodb://localhost:27017/xray_db"
mongo = PyMongo(app)

# Session Configuration
app.config['SESSION_COOKIE_SECURE'] = False  # Set to True in production with HTTPS
app.config['SESSION_COOKIE_HTTPONLY'] = True
app.config['SESSION_COOKIE_SAMESITE'] = 'Lax'
app.config['PERMANENT_SESSION_LIFETIME'] = 1800  # 30 minutes
app.secret_key = 'f3a1d9b5c7e4820d9f14a6c2b7e85f0a'  # Needed for session management

CORS(app, resources={
    r"/*": {
        "origins": [
            "http://127.0.0.1:5000",
            "http://localhost:5000",
            "http://127.0.0.1:8080",
            "http://localhost:8080",
            "http://xrayapp.local:8080"
        ],
        "methods": ["GET", "POST", "OPTIONS"],
        "allow_headers": ["Content-Type", "Accept", "Authorization"],
        "supports_credentials": True,
        "expose_headers": ["Content-Type", "X-CSRFToken"],
        "max_age": 600
    }
})

# Add session debugging
@app.before_request
def before_request():
    print("Session contents:", dict(session))

# Signup route
@app.route('/signup', methods=['POST'])
def signup():
    data = request.get_json()
    name = data.get('name')
    email = data.get('email')
    password = data.get('password')

    if not all([name, email, password]):
        return jsonify({'error': 'Please fill all fields'}), 400

    if mongo.db.users.find_one({'email': email}):
        return jsonify({'error': 'Email already registered'}), 400

    hashed_password = generate_password_hash(password)
    user = {
        'name': name,
        'email': email,
        'password': hashed_password,
        'created_at': datetime.utcnow()
    }
    result = mongo.db.users.insert_one(user)
    
    # Create a user object without the password
    user_obj = {
        'id': str(result.inserted_id),
        'name': name,
        'email': email
    }
    
    # Set session
    session['user'] = user_obj
    
    return jsonify({
        'message': 'User registered successfully',
        'user': user_obj
    }), 200

# Login route
@app.route('/login', methods=['POST'])
def login():
    data = request.get_json()
    email = data.get('email')
    password = data.get('password')

    if not all([email, password]):
        return jsonify({'error': 'Please fill all fields'}), 400

    user = mongo.db.users.find_one({'email': email})
    if not user or not check_password_hash(user['password'], password):
        return jsonify({'error': 'Invalid email or password'}), 401

    # Create a user object without the password
    user_obj = {
        'id': str(user['_id']),
        'name': user['name'],
        'email': user['email']
    }
    
    # Set session
    session.permanent = True  # Make session persistent
    session['user'] = user_obj
    print("User logged in:", user_obj)  # Debug print
    
    return jsonify({
        'message': 'Logged in successfully',
        'user': user_obj
    }), 200

# Logout route
@app.route('/logout', methods=['POST'])
def logout():
    session.pop('user', None)
    return jsonify({'message': 'Logged out successfully'}), 200

# Protected route example
@app.route('/profile')
def profile():
    if 'user' not in session:
        return jsonify({'error': 'Unauthorized'}), 401
    return jsonify({'user': session['user']}), 200

app.config['UPLOAD_FOLDER'] = 'uploads'

if not os.path.exists(app.config['UPLOAD_FOLDER']):
    os.makedirs(app.config['UPLOAD_FOLDER'])

MODEL_PATH = 'model/tbp.keras'  # Path to your saved model

CLASS_NAMES = ['COVID-19', 'NORMAL', 'PNEUMONIA', 'TUBERCULOSIS']

print("Initializing model...")
try:
    # Create base model
    base_model = MobileNetV2(
        input_shape=(224, 224, 3),
        include_top=False,
        weights='imagenet'
    )

    # Add classification layers
    model = tf.keras.Sequential([
        base_model,
        tf.keras.layers.GlobalAveragePooling2D(),
        tf.keras.layers.Dense(128, activation='relu'),
        tf.keras.layers.Dropout(0.5),
        tf.keras.layers.Dense(len(CLASS_NAMES), activation='softmax')
    ])

    # Try to load weights
    print(f"Loading model weights from {MODEL_PATH}...")
    model.load_weights(MODEL_PATH)
    print("Model weights loaded successfully!")

    # Compile the model
    model.compile(
        optimizer='adam',
        loss='categorical_crossentropy',
        metrics=['accuracy']
    )
    print("Model compiled successfully!")

except Exception as e:
    print(f"Error initializing model: {str(e)}")
    print("Please ensure the model file exists and is in the correct format")
    raise

# Add route to serve uploaded images
@app.route('/uploads/<filename>')
def serve_image(filename):
    return send_from_directory(app.config['UPLOAD_FOLDER'], filename)

@app.route('/')
def index():
    try:
        return render_template('iindex.html')
    except Exception as e:
        print(f"Error rendering template: {str(e)}")
        return str(e), 500

@app.route('/static/<path:filename>')
def serve_static(filename):
    return send_from_directory(app.static_folder, filename)

@app.route('/database')
def view_database():
    # Fetch summary statistics
    total_users = mongo.db.users.count_documents({})
    total_analyses = mongo.db.images.count_documents({})
    
    # Calculate average confidence
    confidence_pipeline = [
        {
            '$group': {
                '_id': None,  # Group all documents together
                'average_confidence': {'$avg': '$confidence'}
            }
        }
    ]
    avg_confidence_result = list(mongo.db.images.aggregate(confidence_pipeline))
    average_confidence = avg_confidence_result[0]['average_confidence'] if avg_confidence_result else 0.0

    # Fetch all images for display in the table
    images = list(mongo.db.images.find({}).sort('upload_date', -1))
    current_year = datetime.utcnow().year # Get the actual current year from the server

    for image in images:
        # Check if the upload_date year is in the future and correct it
        if image['upload_date'].year > current_year:
            image['upload_date'] = image['upload_date'].replace(year=current_year)

    # Fetch all users for display in the users table
    users = list(mongo.db.users.find({}).sort('created_at', -1))

    for user in users:
        # Check if the created_at year is in the future and correct it
        if user['created_at'].year > current_year:
            user['created_at'] = user['created_at'].replace(year=current_year)

    return render_template(
        'database_view.html',
        total_users=total_users,
        total_analyses=total_analyses,
        average_confidence=average_confidence,
        images=images,
        users=users
    )

@app.route('/user_diagnosis_history')
def user_diagnosis_history():
    if 'user' not in session:
        return jsonify({'error': 'Unauthorized'}), 401

    user_id = session['user']['id']
    images = list(mongo.db.images.find({'user_id': user_id}).sort('upload_date', -1))

    current_year = datetime.utcnow().year
    for image in images:
        image['_id'] = str(image['_id'])
        image['user_id'] = str(image['user_id'])
        image['upload_date'] = image['upload_date'].isoformat()
        # Correct year if it's in the future
        if datetime.fromisoformat(image['upload_date']).year > current_year:
            dt_obj = datetime.fromisoformat(image['upload_date'])
            image['upload_date'] = dt_obj.replace(year=current_year).isoformat()
    
    return jsonify({'images': images})

@app.route('/predict', methods=['POST', 'OPTIONS'])
def predict():
    if request.method == 'OPTIONS':
        response = jsonify({'status': 'ok'})
        response.headers.add('Access-Control-Allow-Origin', '*')
        response.headers.add('Access-Control-Allow-Headers', 'Content-Type')
        response.headers.add('Access-Control-Allow-Methods', 'POST')
        return response
        
    if 'file' not in request.files:
        return jsonify({'error': 'No file part'}), 400

    file = request.files['file']
    if file.filename == '':
        return jsonify({'error': 'No file selected'}), 400

    if not file:
        return jsonify({'error': 'Something went wrong'}), 500

    try:
        # Create uploads directory if it doesn't exist
        if not os.path.exists(app.config['UPLOAD_FOLDER']):
            os.makedirs(app.config['UPLOAD_FOLDER'])

        # Generate a unique filename
        filename = f"{int(time.time())}_{secure_filename(file.filename)}"
        filepath = os.path.join(app.config['UPLOAD_FOLDER'], filename)
        file.save(filepath)

        try:
            # Load and preprocess the image
            img = Image.open(filepath).convert('RGB')
            
            # Simple heuristic: check if image is mostly grayscale
            img_array_check = np.array(img)
            if np.std(img_array_check[..., 0] - img_array_check[..., 1]) < 5 and \
               np.std(img_array_check[..., 1] - img_array_check[..., 2]) < 5:
                # Looks grayscale → proceed
                pass
            else:
                # Not grayscale → likely not an X-ray
                os.remove(filepath)  # Clean up uploaded file
                return jsonify({'error': 'Invalid image: not an X-ray'}), 400

            # Process image if it passes grayscale check
            img = img.resize((224, 224))
            img_array = np.array(img)
            img_array = np.expand_dims(img_array, axis=0)
            img_array = preprocess_input(img_array)

            # Predict
            predictions = model.predict(img_array, verbose=0)
            predicted_class = CLASS_NAMES[np.argmax(predictions[0])]
            confidence = float(np.max(predictions[0]) * 100)

            # Store image information in MongoDB
            if 'user' in session:
                image_data = {
                    'user_id': session['user']['id'],
                    'filename': filename,
                    'prediction': predicted_class,
                    'confidence': confidence,
                    'upload_date': datetime.utcnow()
                }
                mongo.db.images.insert_one(image_data)

            return jsonify({
                'prediction': predicted_class,
                'confidence': confidence,
                'filename': filename
            }), 200

        except Exception as e:
            print(f"Error processing image: {str(e)}")
            if os.path.exists(filepath):
                os.remove(filepath)
            return jsonify({'error': 'Error processing image'}), 500

    except Exception as e:
        print(f"Error saving file: {str(e)}")
        return jsonify({'error': 'Error saving file'}), 500

if __name__ == '__main__':
    app.run(debug=True, port=8080, host='0.0.0.0')
