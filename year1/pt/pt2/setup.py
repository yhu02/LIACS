import setuptools

setuptools.setup(
    name="pt2-webapp-s2962802-Yvo-Hu", # Replace with your own username
    version="0.0.1",
    author="Yvo Hu",
    author_email="s2962802@vuw.leidenuniv.com",
    description="Een webapp",
    url="https://git.liacs.nl/s2962802/pt2",
    project_urls={
        "pt2": "https://git.liacs.nl/s2962802/pt2",
    },
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: Linux",
    ],
    package_dir={"": "app"},
    packages=setuptools.find_packages(where="app"),
    python_requires=">=3.6",
    install_requires=[
        'Flask_Login',
        'Flask_Migrate',
        'Flask_SQLAlchemy',
        'Werkzeug',
        'dataclasses',
        'Flask_SocketIO',
        'WTForms',
        'Flask_WTF',
        'SQLAlchemy',
        'Flask',
        'alembic',
        'email-validator',
        'eventlet',
        'dnspython==1.16.0' ]
)
