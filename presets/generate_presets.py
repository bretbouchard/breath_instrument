#!/usr/bin/env python3
"""
generate_presets.py - Generate JUCE XML preset files for Breath Lead
"""

import os
import xml.etree.ElementTree as ET
from xml.dom import minidom

def create_preset(name, air=0.5, tone=0.6, formant=0.5, resistance=0.4, vibrato=0.0, author="SchillingerEcosystem"):
    """Create a JUCE preset XML file"""
    # Create root element
    root = ET.Element("PRESET")
    root.set("version", "1")
    root.set("name", name)
    root.set("author", author)
    root.set("plugin", "BreathLead")

    # Create values element
    values = ET.SubElement(root, "VALUES")

    # Add parameters
    params = [
        ("air", air),
        ("tone", tone),
        ("formant", formant),
        ("resistance", resistance),
        ("vibrato", vibrato),
    ]

    for param_name, param_value in params:
        param = ET.SubElement(values, "PARAM")
        param.set("id", param_name)
        param.set("value", str(param_value))

    # Pretty print XML
    xml_str = ET.tostring(root, encoding="unicode")
    dom = minidom.parseString(xml_str)
    pretty_xml = dom.toprettyxml(indent="  ")

    # Remove extra blank lines
    lines = [line for line in pretty_xml.split('\n') if line.strip()]
    return '\n'.join(lines)

def save_preset(filename, content):
    """Save preset to file"""
    os.makedirs(os.path.dirname(filename), exist_ok=True)
    with open(filename, 'w') as f:
        f.write(content)
    print(f"✅ Created: {filename}")

def main():
    print("🎵 Generating Breath Lead Presets\n")

    # Preset definitions
    presets = [
        # === Init Patches ===
        {
            "name": "Golden Init Patch",
            "category": "Init",
            "params": {
                "air": 0.5,
                "tone": 0.6,
                "formant": 0.5,
                "resistance": 0.4,
                "vibrato": 0.0,
            },
            "description": "Default patch - sounds musical immediately"
        },

        # === Breath Intensity ===
        {
            "name": "Soft Breath",
            "category": "Breath",
            "params": {
                "air": 0.3,
                "tone": 0.5,
                "formant": 0.5,
                "resistance": 0.6,
                "vibrato": 0.0,
            },
            "description": "Gentle, intimate breath"
        },
        {
            "name": "Medium Breath",
            "category": "Breath",
            "params": {
                "air": 0.5,
                "tone": 0.6,
                "formant": 0.5,
                "resistance": 0.4,
                "vibrato": 0.0,
            },
            "description": "Balanced, versatile"
        },
        {
            "name": "Full Breath",
            "category": "Breath",
            "params": {
                "air": 0.8,
                "tone": 0.7,
                "formant": 0.5,
                "resistance": 0.3,
                "vibrato": 0.0,
            },
            "description": "Powerful, projecting"
        },

        # === Tone Colors ===
        {
            "name": "Dark",
            "category": "Tone",
            "params": {
                "air": 0.5,
                "tone": 0.2,
                "formant": 0.5,
                "resistance": 0.4,
                "vibrato": 0.0,
            },
            "description": "Warm, muted, intimate"
        },
        {
            "name": "Bright",
            "category": "Tone",
            "params": {
                "air": 0.5,
                "tone": 0.9,
                "formant": 0.5,
                "resistance": 0.4,
                "vibrato": 0.0,
            },
            "description": "Clear, present, cutting"
        },
        {
            "name": "Neutral",
            "category": "Tone",
            "params": {
                "air": 0.5,
                "tone": 0.5,
                "formant": 0.5,
                "resistance": 0.4,
                "vibrato": 0.0,
            },
            "description": "Balanced, natural"
        },

        # === Character ===
        {
            "name": "Airy",
            "category": "Character",
            "params": {
                "air": 0.7,
                "tone": 0.6,
                "formant": 0.3,
                "resistance": 0.2,
                "vibrato": 0.0,
            },
            "description": "Breath noise prominent, spacious"
        },
        {
            "name": "Focused",
            "category": "Character",
            "params": {
                "air": 0.5,
                "tone": 0.6,
                "formant": 0.7,
                "resistance": 0.8,
                "vibrato": 0.0,
            },
            "description": "Tight, controlled, centered"
        },
        {
            "name": "Open",
            "category": "Character",
            "params": {
                "air": 0.6,
                "tone": 0.7,
                "formant": 0.4,
                "resistance": 0.2,
                "vibrato": 0.0,
            },
            "description": "Loose, resonant, free"
        },

        # === Performance ===
        {
            "name": "Subtle Vibrato",
            "category": "Performance",
            "params": {
                "air": 0.5,
                "tone": 0.6,
                "formant": 0.5,
                "resistance": 0.4,
                "vibrato": 0.2,
            },
            "description": "Gentle oscillation, warmth"
        },
        {
            "name": "Wide Vibrato",
            "category": "Performance",
            "params": {
                "air": 0.5,
                "tone": 0.6,
                "formant": 0.5,
                "resistance": 0.4,
                "vibrato": 0.7,
            },
            "description": "Expressive wobble, classic"
        },
        {
            "name": "No Vibrato",
            "category": "Performance",
            "params": {
                "air": 0.5,
                "tone": 0.6,
                "formant": 0.5,
                "resistance": 0.4,
                "vibrato": 0.0,
            },
            "description": "Pure tone, steady"
        },

        # === Musical Styles ===
        {
            "name": "Flute-like",
            "category": "Style",
            "params": {
                "air": 0.6,
                "tone": 0.7,
                "formant": 0.6,
                "resistance": 0.5,
                "vibrato": 0.3,
            },
            "description": "Woodwind character, airy"
        },
        {
            "name": "Brassy",
            "category": "Style",
            "params": {
                "air": 0.7,
                "tone": 0.8,
                "formant": 0.5,
                "resistance": 0.6,
                "vibrato": 0.4,
            },
            "description": "Trumpet-like, bright"
        },
        {
            "name": "Vocal Ooh",
            "category": "Style",
            "params": {
                "air": 0.5,
                "tone": 0.5,
                "formant": 0.4,
                "resistance": 0.5,
                "vibrato": 0.5,
            },
            "description": "Vowel Ooh, warm"
        },
        {
            "name": "Vocal Aah",
            "category": "Style",
            "params": {
                "air": 0.5,
                "tone": 0.6,
                "formant": 0.6,
                "resistance": 0.4,
                "vibrato": 0.5,
            },
            "description": "Vowel Aah, open"
        },
        {
            "name": "Synth Lead",
            "category": "Style",
            "params": {
                "air": 0.4,
                "tone": 0.8,
                "formant": 0.5,
                "resistance": 0.3,
                "vibrato": 0.6,
            },
            "description": "Electronic, controlled"
        },

        # === Special Effects ===
        {
            "name": "Whisper",
            "category": "FX",
            "params": {
                "air": 0.9,
                "tone": 0.3,
                "formant": 0.3,
                "resistance": 0.1,
                "vibrato": 0.0,
            },
            "description": "Noisy, intimate"
        },
        {
            "name": "Punchy",
            "category": "FX",
            "params": {
                "air": 0.8,
                "tone": 0.7,
                "formant": 0.6,
                "resistance": 0.7,
                "vibrato": 0.0,
            },
            "description": "Fast attack, percussive"
        },
        {
            "name": "Evanescent",
            "category": "FX",
            "params": {
                "air": 0.3,
                "tone": 0.8,
                "formant": 0.4,
                "resistance": 0.2,
                "vibrato": 0.3,
            },
            "description": "Delicate, fading"
        },

        # === Additional Expressive Presets ===
        {
            "name": "Clarinet-like",
            "category": "Style",
            "params": {
                "air": 0.5,
                "tone": 0.6,
                "formant": 0.5,
                "resistance": 0.6,
                "vibrato": 0.2,
            },
            "description": "Warm clarinet character, soft chalumeau"
        },
        {
            "name": "Saxophone-like",
            "category": "Style",
            "params": {
                "air": 0.7,
                "tone": 0.7,
                "formant": 0.5,
                "resistance": 0.5,
                "vibrato": 0.6,
            },
            "description": "Jazz sax, expressive vibrato"
        },
        {
            "name": "Recorder",
            "category": "Style",
            "params": {
                "air": 0.4,
                "tone": 0.8,
                "formant": 0.7,
                "resistance": 0.8,
                "vibrato": 0.0,
            },
            "description": "Early music, pure tone"
        },
        {
            "name": "Bagpipes",
            "category": "Style",
            "params": {
                "air": 0.9,
                "tone": 0.5,
                "formant": 0.4,
                "resistance": 0.9,
                "vibrato": 0.0,
            },
            "description": "Drone-like, constant pressure"
        },
        {
            "name": "Theremin-esque",
            "category": "Style",
            "params": {
                "air": 0.4,
                "tone": 0.8,
                "formant": 0.3,
                "resistance": 0.2,
                "vibrato": 0.8,
            },
            "description": "Ethereal, otherworldly"
        },
        {
            "name": "Oboe-like",
            "category": "Style",
            "params": {
                "air": 0.6,
                "tone": 0.7,
                "formant": 0.6,
                "resistance": 0.7,
                "vibrato": 0.4,
            },
            "description": "Double reed, nasal quality"
        },
        {
            "name": "Vocal Hum",
            "category": "Style",
            "params": {
                "air": 0.4,
                "tone": 0.4,
                "formant": 0.5,
                "resistance": 0.6,
                "vibrato": 0.3,
            },
            "description": "Humming, warm resonance"
        },
        {
            "name": "Bass Flute",
            "category": "Style",
            "params": {
                "air": 0.7,
                "tone": 0.3,
                "formant": 0.4,
                "resistance": 0.5,
                "vibrato": 0.4,
            },
            "description": "Low register, dark and airy"
        },
        {
            "name": "Pipe Organ",
            "category": "Style",
            "params": {
                "air": 0.6,
                "tone": 0.6,
                "formant": 0.8,
                "resistance": 0.9,
                "vibrato": 0.0,
            },
            "description": "Church organ, steady tone"
        },
        {
            "name": "Accordion",
            "category": "Style",
            "params": {
                "air": 0.5,
                "tone": 0.5,
                "formant": 0.5,
                "resistance": 0.3,
                "vibrato": 0.0,
            },
            "description": "Reed-like, folk character"
        },
        {
            "name": "Shakuhachi",
            "category": "Style",
            "params": {
                "air": 0.8,
                "tone": 0.4,
                "formant": 0.3,
                "resistance": 0.4,
                "vibrato": 0.5,
            },
            "description": "Japanese bamboo flute, breathy"
        },
        {
            "name": "Didgeridoo",
            "category": "Style",
            "params": {
                "air": 0.9,
                "tone": 0.2,
                "formant": 0.2,
                "resistance": 0.8,
                "vibrato": 0.1,
            },
            "description": "Drone, throaty, primal"
        },
    ]

    # Generate presets
    output_dir = "presets"
    categories = {}

    for preset in presets:
        category = preset["category"]
        name = preset["name"]
        description = preset["description"]
        params = preset["params"]

        # Create category directory
        category_dir = os.path.join(output_dir, category)
        os.makedirs(category_dir, exist_ok=True)

        # Generate preset XML
        xml_content = create_preset(
            name=name,
            air=params["air"],
            tone=params["tone"],
            formant=params["formant"],
            resistance=params["resistance"],
            vibrato=params["vibrato"],
        )

        # Save preset
        filename = os.path.join(category_dir, f"{name}.xml")
        save_preset(filename, xml_content)

        # Track by category
        if category not in categories:
            categories[category] = []
        categories[category].append(name)

    # Generate index
    print("\n" + "="*60)
    print("📚 Preset Library Index")
    print("="*60 + "\n")

    for category in sorted(categories.keys()):
        print(f"\n{category}")
        print("-" * len(category))
        for preset_name in categories[category]:
            print(f"  • {preset_name}")

    print(f"\n{'='*60}")
    print(f"✅ Generated {len(presets)} presets across {len(categories)} categories")
    print(f"{'='*60}\n")

    print("📁 Output directory: presets/")
    print("\n📖 Installation instructions:")
    print("   Copy preset folders to:")
    print("   ~/Library/Audio/Presets/SchillingerEcosystem/BreathLead/")
    print()

if __name__ == "__main__":
    main()
